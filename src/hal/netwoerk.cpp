#include "../include/hal/network.h"

bool SmartSecurity_Esp::Network::connectSTA(const char* ssid, const char* password) {
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);
            while (WiFi.status()!= WL_CONNECTED) {
                Serial.print(".");
                delay(500);
            }
            Serial.println("");
            Serial.print("Connected to ");
            Serial.println(ssid);
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            return true;
}
void SmartSecurity_Esp::Network::connectTCP(const char* ipaddress, int port){
    
    while(!this->client.connect(ipaddress, port)){
        Serial.println("try to connecting to server...");
        delay(500);
    }
    Serial.println("connected to server successfully");
}
void SmartSecurity_Esp::Network::SendMessageTest(const char* topic, const char* message){
    byte header[40] = {0}; // 全部初始化为0
    // 填充topic（最多32字节）
    size_t topicLen = strlen(topic);
    if (topicLen > 32) { // 强制长度限制
        topicLen = 32;
    }
    memcpy(header, topic, topicLen);
     // 小端序写入topic长度（4字节）
    uint32_t topicLenLE = htole32(topicLen);
    memcpy(header + 32, &topicLenLE, 4);
    
    // 小端序写入消息体长度（4字节）
    uint32_t msgLen = strlen(message);
    uint32_t msgLenLE = htole32(msgLen);
    memcpy(header + 36, &msgLenLE, 4);

    // 合并发送消息头+消息体
    client.write(header, sizeof(header)); // 发送40字节头
    client.write((const uint8_t*)message, msgLen); // 发送消息体
    // client.stop();
}
void SmartSecurity_Esp::Network::SendImage(const char* topic){
    static unsigned long lastFrame = 0;
    const int interval = 33;     
     
    if(millis() - lastFrame >= interval){
        camera_fb_t *fb = esp_camera_fb_get();
        if (fb) {
            byte header[40] = {0}; // 全部初始化为0
            // 填充topic（最多32字节）
            size_t topicLen = strlen(topic);
            if (topicLen > 32) { // 强制长度限制
                topicLen = 32;
            }
            memcpy(header, topic, topicLen);
            // 小端序写入topic长度（4字节）
            uint32_t topicLenLE = htole32(topicLen);
            memcpy(header + 32, &topicLenLE, 4);
            // 小端序写入消息体长度（4字节）
            uint32_t msgLenLE = htole32(fb->len);
            memcpy(header + 36, &msgLenLE, 4);
            // 合并发送消息头+消息体长度
            client.write(header, sizeof(header)); // 发送40字节头
            // 发送消息体
            client.write(fb->buf, fb->len); // 发送消息体
            //分块发送
            // size_t sent = 0;
            // while(sent < fb->len){
            //     size_t chunk = min((size_t)1024*10, fb->len - sent);
            //     size_t wrote = client.write(fb->buf + sent, chunk);
            //     if (wrote == 0) {
            //         Serial.println("发送中断，准备重连");
            //         client.stop();
                    
            //         break;
            //     }
            //     sent += wrote;
            // }
        }
        esp_camera_fb_return(fb);
        lastFrame = millis();
    }
    // client.write((const uint8_t*)fb->buf, fb->len); // 发送消息体
    // client.stop();
    
}
     
