#include <WiFi.h>
#include <OSCMessage.h>

const char *ssid = "UMZU";
const char *password = "45384685136386595053";

IPAddress broadcastIP;
bool broadcastIPSet = false;

static const IPAddress espIp(192, 168, 2, 233);

const int buttonPin = 4;      // GPIO D4
const int ledPin = 2;         // GPIO D2
const int debounceDelay = 50; // Debounce delay in milliseconds

int buttonState = HIGH;             // Current state of the button
int lastButtonState = HIGH;         // Previous state of the button
unsigned long lastDebounceTime = 0; // Last time the button state changed

WiFiUDP udp;

IPAddress getBroadcastIP()
{
  IPAddress localIP = WiFi.localIP();
  IPAddress subnetMask = WiFi.subnetMask();
  IPAddress broadcastIP;

  broadcastIP = localIP | ~subnetMask;

  return broadcastIP;
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  broadcastIP = getBroadcastIP();
  broadcastIPSet = true;
  Serial.println("Broadcast IP: ");
  Serial.println(broadcastIP);
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  broadcastIPSet = false;
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

void setup()
{
  Serial.begin(115200);

  // delete old config
  WiFi.disconnect(true);

  delay(1000);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.config(espIp, IPAddress(0, 0, 0, 0), IPAddress(255, 255, 255, 0));

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");

  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(ledPin, OUTPUT);          
  udp.begin(9000);
}



void loop()
{
  if (!broadcastIPSet)
  {
    digitalWrite(ledPin, LOW);
    return;
  }

  digitalWrite(ledPin, HIGH);
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading; // Update the button state

      Serial.println("Button state changed to:");
      Serial.println(buttonState);

      // Send button state via OSC
      OSCMessage message("/Mx/playback/page1/0/go");
      message.add(buttonState);
      udp.beginPacket(broadcastIP, 8000);
      message.send(udp);
      udp.endPacket();
    }
  }

  lastButtonState = reading; // Save the current button state
}
