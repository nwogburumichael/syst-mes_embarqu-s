#include <Arduino.h>

#include "FS.h"
#include <LittleFS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHT11PIN 26
DHT dht(DHT11PIN, DHT11);
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>


// Variables pour la connexion wifi 
const char* ssid = "Wifi-Abosi";
const char* password = "Nwogburu234";
const char* PARAM_MESSAGE = "message";

// Variables Get
String inputMessage = "";
String inputMessage2 = "";
const char* PARAM_INPUT_1 = "";
const char* PARAM_INPUT_2 = "";

// Variables pour le client
float humi;
float temperature;
bool LEDonoff = false;
int NB = 0;
String valeur_bp = "OFF";
String JSONtxt ="";
 
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server");
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
 
  }
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// Evenement du Websocket server
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  String Donnees_recue = (const char *)payload;
  Serial.print("Données recue = ");
  Serial.println(Donnees_recue);

// Reception des données du client
  if(type == WStype_TEXT) 

  {
      byte separator=Donnees_recue.indexOf('=');
      String var = Donnees_recue.substring(0,separator);
      Serial.print("var= ");
      Serial.println(var);
      String val = Donnees_recue.substring(separator+1);
      Serial.print("val= ");
      Serial.println(val);
      Serial.println(" ");

      if(var == "LEDonoff")
        {
          LEDonoff = false;
          if(val == "ON") LEDonoff = true;
        }
  }
}

void setup() {
 
    Serial.begin(115200);

    // Connexion Wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
 
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    dht.begin();

    // Implémentation de litlle FS
   if(!LITTLEFS.begin(true)){
    Serial.println("An Error has occurred while mounting LITTLEFS");
    }
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LITTLEFS, "/index.html","text/html",false); 
    });

    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LITTLEFS, "/index.css","text/css",false); 
    });

    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LITTLEFS, "/index.js","text/js",false); 
    });

  // Receive an HTTP GET request at <ESP_IP>/get?threshold_input=<inputMessage>&enable_arm_input=<inputMessage2>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
       
      }
    
    }
    Serial.println(inputMessage);
    Serial.println(inputMessage2);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
 
    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });
 
    server.onNotFound(notFound);
  
    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
   
}
 
void loop() {
   webSocket.loop();
   humi = dht.readHumidity();
   temperature = dht.readTemperature();
   Serial.println(temperature) ;
   
   delay(2000);
   JSONtxt = "{\"humi\":\""+String(humi)+"\",\"temperature\":\""+String(temperature)+"\",\"valeur_bp\":\""+valeur_bp+"\"}";
   webSocket.broadcastTXT(JSONtxt);

}