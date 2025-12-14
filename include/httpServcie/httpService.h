#ifndef _HTTPSERVICE_H_
#define _HTTPSERVICE_H_
#include <HTTPClient.h>
#include <esp_camera.h>
#include <Arduino.h>
namespace SmartSecurity_Esp {
    class HttpService {
        public:
            HttpService();
            ~HttpService()=default;
            bool requestPost(const char* url, const char* data);
            void HttpTest();
            int postImgTest();

    };

}


#endif
