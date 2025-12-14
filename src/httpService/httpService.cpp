#include "../include/httpServcie/httpService.h"





SmartSecurity_Esp::HttpService::HttpService(){
}

bool SmartSecurity_Esp::HttpService::requestPost(const char *url, const char *data){
    HTTPClient http;
    http.begin(url);
    int httpCode = http.POST(data);
    return false;
}
void SmartSecurity_Esp::HttpService::HttpTest(){
     HTTPClient http;
     http.begin("http://192.168.137.1:8080/HAL/esp32/test");
     int httpCode = http.POST("");
     String payload = http.getString();
     Serial.print("HTTP Response code: "+httpCode);
     Serial.println();
     Serial.println(payload);
     http.end();
}
int SmartSecurity_Esp::HttpService::postImgTest() {
      HTTPClient http;
    http.begin("http://192.168.137.1:8080/HAL/esp32/facial-recognition/7");
    
    // 设置multipart/form-data请求头
     String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
     http.addHeader("Content-Type", "multipart/form-data; boundary="+boundary);

    
    // 读取图片文件（示例使用假数据）
    camera_fb_t *fb = esp_camera_fb_get();
    if(!fb) {
        Serial.println("Camera capture failed");
        return -1;
    }
    String body = "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n";
    body += "Content-Type: image/jpeg\r\n\r\n";
    body += String((char*)fb->buf, fb->len);
    body += "\r\n--" + boundary + "--\r\n";
   
    int httpCode = http.POST((uint8_t*)body.c_str(), body.length());
    esp_camera_fb_return(fb);
    http.end();
    return httpCode;
}