#ifndef _CAM_H_
#define _CAM_H_
#include "esp_camera.h"
#include <Arduino.h>
#include <string>
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13
namespace SmartSecurity_Esp {
       class Cam {
            private:
                camera_config_t config;
            public:
                void CamInit();
                void SetCameraConfig(String config){
                    if(config=="MEDIUM_RESOLUTION"){
                        this->config.frame_size = FRAMESIZE_QVGA;
                        Serial.println("Set camera resolution to SVGA (800x600)");
                    }else if(config=="LOW_RESOLUTION"){
                        this->config.frame_size = FRAMESIZE_VGA;
                        Serial.println("Set camera resolution to QQVGA (160x120)");
                    }

                    esp_camera_deinit();
                    esp_err_t err = esp_camera_init(&this->config);
                    if (err != ESP_OK) {
                        Serial.printf("Camera re-init failed with error 0x%x", err);
                        return;
                    }
                    Serial.println("Camera config updated successfully");
                }
                ~Cam()=default;
       };
}

#endif /* _CAM_H_ */