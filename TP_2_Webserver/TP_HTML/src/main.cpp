#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
 
const char* ssid = "LAPTOP_T";
const char* password = "TIMON123";
 
const char* PARAM_MESSAGE = "message";
 
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
 
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_DATA)
  {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len)
    {
      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      }
 
      Serial.printf("%s\n",msg.c_str());
    }
  }
}
 
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
 
void setup() {
 
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
 
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<!DOCTYPE html><body><h1>TEST TEST</h1><p>Hello World</p></body></html>");
    });
 
    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });
 
    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });
 
    server.onNotFound(notFound);
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.begin();
}
 
void loop() {
  ws.textAll("Coucou");
  delay(1000);
}