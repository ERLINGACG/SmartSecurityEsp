#ifndef __NETWORK_H__
#define __NETWORK_H__
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include "cam.h"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define htole32(x) (x)
#else
    #define htole32(x) __builtin_bswap32(x)
#endif
namespace SmartSecurity_Esp {
    struct agreement{
       
    };
    class Network {
        private:
          WiFiClient client;
        public:
            ~Network()=default;
            static bool connectSTA(const char* ssid, const char* password);
            void connectTCP(const char* ipaddress,int port);
            void SendMessageTest(const char* topic, const char* message);
            void SendImage(const char* topic);
    };
}
#endif //__NETWORK_H__