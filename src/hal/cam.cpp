#include "../include/hal/cam.h"
void SmartSecurity_Esp::Cam::CamInit() {
    this->config.ledc_channel = LEDC_CHANNEL_0;
    this->config.ledc_timer = LEDC_TIMER_0;
    this->config.pin_d0 = Y2_GPIO_NUM;
    this->config.pin_d1 = Y3_GPIO_NUM;
    this->config.pin_d2 = Y4_GPIO_NUM;
    this->config.pin_d3 = Y5_GPIO_NUM;
    this->config.pin_d4 = Y6_GPIO_NUM;
    this->config.pin_d5 = Y7_GPIO_NUM;
    this->config.pin_d6 = Y8_GPIO_NUM;
    this->config.pin_d7 = Y9_GPIO_NUM;
    this->config.pin_xclk = XCLK_GPIO_NUM;
    this->config.pin_pclk = PCLK_GPIO_NUM;
    this->config.pin_vsync = VSYNC_GPIO_NUM;
    this->config.pin_href = HREF_GPIO_NUM;
    this->config.pin_sccb_sda = SIOD_GPIO_NUM;    // 新字段
    this->config.pin_sccb_scl = SIOC_GPIO_NUM;     // 新字段
  
    this->config.pin_pwdn = PWDN_GPIO_NUM;
    this->config.pin_reset = RESET_GPIO_NUM;
    this->config.xclk_freq_hz = 20000000;
    // 硬件降噪配置
    sensor_t *s = esp_camera_sensor_get();
    if(s != NULL){
        s->set_denoise(s, 1);               // 启用硬件降噪
        s->set_brightness(s, 4);             // 亮度调节
        s->set_gainceiling(s, GAINCEILING_8X);
        s->set_saturation(s, 0);             // 饱和度调整
        s->set_awb_gain(s, 1);               // 自动白平衡增益
    }

    this->config.frame_size =   FRAMESIZE_QVGA;
    //可用：FRAMESIZE_QVGA
    this->config.pixel_format = PIXFORMAT_JPEG; // for streaming
 
    this->config.grab_mode =   CAMERA_GRAB_WHEN_EMPTY;
    this->config.fb_location = CAMERA_FB_IN_PSRAM;
    this->config.jpeg_quality = 15;
    this->config.fb_count = 2;
    if(psramFound()){
        Serial.println("PSRAM IC found");
    }
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        ESP.restart();
        return;
    }else{
        Serial.println("Camera init success");
    } 
}