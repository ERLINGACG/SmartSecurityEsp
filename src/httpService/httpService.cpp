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
      String rawTopic = "/topic/image2";
      http.begin("http://10.100.88.172:8080/group/member/verifyDnnSSDcaffem/path");

      // 设置multipart/form-data请求头
      String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
      http.addHeader("Content-Type", "multipart/form-data; boundary="+boundary);
      http.addHeader("topic",rawTopic);
    
      // 读取图片文件
      camera_fb_t *fb = esp_camera_fb_get();
      if(!fb) {
          Serial.println("Camera capture failed");
          return -1;
      }

      // ========== 以下是修改后的核心段 ==========
      // 1. 定义固定头部/尾部，计算长度
      const char* header = ("--" + boundary + "\r\n"
                           "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n"
                           "Content-Type: image/jpeg\r\n\r\n").c_str();
      const char* footer = ("\r\n--" + boundary + "--\r\n").c_str();
      size_t headerLen = strlen(header);
      size_t footerLen = strlen(footer);
      size_t totalLen = headerLen + fb->len + footerLen;

      // 2. 分配缓冲区存储完整请求体
      uint8_t* postData = (uint8_t*)malloc(totalLen);
      if (postData == NULL) {
          Serial.println("Malloc failed (out of memory)");
          esp_camera_fb_return(fb);
          http.end();
          return -2;
      }

      // 3. 分段拷贝数据（避免String处理二进制）
      memcpy(postData, header, headerLen);
      memcpy(postData + headerLen, fb->buf, fb->len);
      memcpy(postData + headerLen + fb->len, footer, footerLen);

      // 4. 执行POST
      int httpCode = http.POST(postData, totalLen);

      // 5. 释放缓冲区
      free(postData);
      // ========== 修改结束 ==========

      esp_camera_fb_return(fb);
      String response = http.getString();
      Serial.println("Server Response:");
      Serial.println(response);
      http.end();
   
      return httpCode;
}