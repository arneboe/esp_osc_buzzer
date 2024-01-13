// #include <OSCMessage.h>
// #include <Arduino.h>
// #include <WiFi.h>
// #include <vector>
// #include "esp_wifi.h"

// // static const char *ssid = "Lichtausgang_Buzzer";
// // static const char *password = "lichtanlichtaus";
// // static const IPAddress apLocalIp(192, 168, 1, 77);
// // static const IPAddress apGateway(192, 168, 1, 254);
// // static const IPAddress apSubnet(255, 255, 255, 0);
// WiFiUDP udp;
// bool wifiConnected = false;

// IPAddress getBroadcastIP()
// {
//   IPAddress localIP = WiFi.localIP();
//   IPAddress subnetMask = WiFi.subnetMask();
//   IPAddress broadcastIP;

//   broadcastIP = localIP | ~subnetMask;

//   return broadcastIP;
// }

// void setup()
// {
//   WiFi.useStaticBuffers(true);

//   WiFi.begin("Lichtausgang", "lichtanlichtaus"); // Connect to WiFi network
//   wifiConnected = WiFi.waitForConnectResult(1000) == WL_CONNECTED;
//   if (wifiConnected)
//   {
//     Serial.print("Connected to WiFi. IP address: ");
//     Serial.println(WiFi.localIP()); // Print the received IP via DHCP to Serial
//     Serial.println("Broadcast IP: ");
//     Serial.println(getBroadcastIP());
//   }

//   Serial.begin(9600);
//   udp.begin(9000);
// }

// // std::vector<IPAddress> getConnectedIps()
// // {
// //   std::vector<IPAddress> ips;
// //   ips.reserve(4);
// //   wifi_sta_list_t wifi_sta_list;
// //   tcpip_adapter_sta_list_t adapter_sta_list;

// //   memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
// //   memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

// //   esp_wifi_ap_get_sta_list(&wifi_sta_list);
// //   tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

// //   for (int i = 0; i < adapter_sta_list.num; i++)
// //   {
// //     tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
// //     ips.emplace_back(station.ip.addr);
// //   }
// //   return ips;
// // }


// bool press = true;

// void loop()
// {
//   if (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.println("WiFi disconnected. Waiting for reconnect...");
//     wifiConnected = false;
//     WiFi.reconnect();
//     WiFi.waitForConnectResult(5000);
//     return;
//   }

//   if (WiFi.status() == WL_CONNECTED && !wifiConnected)
//   {
//     Serial.print("Connected to WiFi. IP address: ");
//     Serial.println(WiFi.localIP()); // Print the received IP via DHCP to Serial
//     Serial.println("Broadcast IP: ");
//     Serial.println(getBroadcastIP());
//     wifiConnected = true;
//   }

//   OSCMessage m("/Mx/playback/page1/0/go");
//   m.add(uint8_t(press));
//   press = !press;

//   const auto broadcastTarget = getBroadcastIP();
//   udp.beginPacket(broadcastTarget, 8000);
//   m.send(udp);
//   udp.endPacket();

//   delay(2000);
// }
