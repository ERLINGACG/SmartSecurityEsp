#include <Arduino.h>
#include "../include/hal/cam.h"
#include "../include/hal/network.h"
#include "../include/hal/button.h"
#include "../include/httpServcie/httpService.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <PubSubClient.h>
using namespace SmartSecurity_Esp;

Cam cam;
Network network;
HttpService httpService;

Button button(1);

bool isPressed = true;
// 创建任务函数
void sendImageTask(void *pvParameters) {
  network.SendMessageTest("/topic/image2","keyTest");
  while(true) {
    if(isPressed){
      network.SendImage("/topic/image2");
    }
    
    vTaskDelay(1); // 改为1个tick的延时（约1ms）
    // vTaskDelay(pdMS_TO_TICKS(1000)); // 1秒间隔
  }
  // vTaskDelete(NULL); //完成后删除自身
}
void httpTestTask(void *pvParameters){
  while(1){
  
     vTaskDelay(pdMS_TO_TICKS(1000)); // 5秒间隔
     int httpCode = httpService.postImgTest();
     Serial.println(httpCode);
     Serial.print("Free heap: ");
     Serial.println(ESP.getFreeHeap());
     Serial.print("Free stack: ");
     Serial.println(uxTaskGetStackHighWaterMark(NULL)); // 打印当前任务剩余栈空间
  }
}

void testTask(void *pvParameters){
    int i = 1;
    while(1){
      //  Serial.println("testTask");
      if(button.isPressed()){
        // Serial.println("button pressed:");
       
        i++; 
        // Serial.println(i);  // 添加变量i的输出
      }
       if(i % 2 == 0){
           isPressed = false;
          //  Serial.println("video stop");
          //  vTaskDelete(NULL);
       }else{
           isPressed = true;
          //  Serial.println("video start");
       }
       vTaskDelay(pdMS_TO_TICKS(100)); // 0.1秒间隔
    }
    // vTaskDelete(NULL); // 正确终止任务
}


void mqtt_callback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);
    Serial.print("Message content: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // 将payload转换为字符串以便处理
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    // 检查消息是否为"start"
    if(message == "MEDIUM_RESOLUTION"){
       isPressed = false;
       cam.SetCameraConfig(message);
       isPressed = true;
    }
    // 检查消息是否为"stop"
    else if(message == "LOW_RESOLUTION"){
        isPressed = false;
        cam.SetCameraConfig(message);
         isPressed = true;
    }
}
void mqttTask(void *pvParameters){
    const char* mqtt_client_id = "ESP8266Client_001";
    // const char* mqtt_server = "192.168.1.4";
    // const char* mqtt_server = "192.168.137.1";
    const char* mqtt_server = "10.100.88.172";
    const int mqtt_port = 1883;
    const char* mqtt_topic = "/topic/image2"; 
    
    
    WiFiClient client;
    PubSubClient MQTTclient(client);
    auto mqttTest=[&]()->void{
         MQTTclient.setServer(mqtt_server, mqtt_port);
         MQTTclient.setCallback(mqtt_callback);
    };
    mqttTest();
    if(MQTTclient.connect(mqtt_client_id)){
      Serial.println("MQTT connected");
    }
    while(!MQTTclient.connected()){
        Serial.println("Connecting to MQTT...");
        if(MQTTclient.connect(mqtt_client_id)){
            Serial.println("MQTT connected");
        }else{
            Serial.print("failed, rc=");
            Serial.print(MQTTclient.state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(pdMS_TO_TICKS(5000)); // 5秒后重试
        }
    }
    MQTTclient.subscribe(mqtt_topic);
    while(1){
       
        MQTTclient.loop();
        vTaskDelay(pdMS_TO_TICKS(100)); // 0.1秒间隔
    }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  cam.CamInit();
  // network.connectSTA("4988","00004988");
  // network.connectSTA("西苑201","12345678");
  // network.connectTCP("192.168.1.4",12346);
  // network.connectSTA("r1","123456789");
  network.connectSTA("REDMI K90","31415161624");
  // network.connectTCP("192.168.10.33",12346);
  network.connectTCP("10.100.88.172",12346);
//  network.connectTCP("192.168.137.1",12346);
 
    xTaskCreatePinnedToCore(
      sendImageTask,   // 任务函数
      "SendImageTask", // 任务名称
      4096,            // 堆栈大小
      NULL,            // 参数
      1,               // 优先级
      NULL,            // 任务句柄
      0                // 核心编号（0或1）
    );
    //  xTaskCreatePinnedToCore(
    //   httpTestTask,   // 任务函数
    //   "httpTestTask", // 任务名称
    //   4096,            // 堆栈大小
    //   NULL,            // 参数
    //   1,               // 优先级
    //   NULL,            // 任务句柄
    //   0                // 核心编号（0或1）
    // );
    xTaskCreatePinnedToCore(
      mqttTask,   // 任务函数
      "mqttTask", // 任务名称
      4096,            // 堆栈大小
      NULL,            // 参数
      1,               // 优先级
      NULL,            // 任务句柄
      0                // 核心编号（0或1）
    );
 
}

void loop() {
  
}
