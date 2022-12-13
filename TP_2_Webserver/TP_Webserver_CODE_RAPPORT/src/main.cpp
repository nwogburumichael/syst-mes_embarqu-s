#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FS.h"
#include <LittleFS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// DHT
#define DHT22PIN 26
DHT dht(DHT22PIN, DHT22);

//Oled 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variables pour la connexion wifi 
const char* ssid = "Wifi-Abosi";
const char* password = "Nwogburu234";
const char* PARAM_MESSAGE = "message";

// Variables Get
String inputMessage = "";
const char* PARAM_INPUT_1 = "temperature";
const char* PARAM_INPUT_2 = "humi";
const char* PARAM_INPUT_3 = "valeur_bp";

// Variables pour le client
#define BP1 12
#define LED_Statut 32
#define LED_Client 2
float humi;
float temperature;

String valeur_bp = "OFF";
String JSONtxt ="";
String valeur_LED_S ="OFF";
 
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void affichage_oled(){
if (digitalRead(BP1) == 1)
    {
      valeur_bp = "ON";
    }
  else {
     valeur_bp ="OFF";
  }
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(5, 25);
display.println(WiFi.localIP());
display.setCursor(5,35);
display.println("BP:"+valeur_bp);
display.setCursor(5,45);
display.println("LED:"+valeur_LED_S);
display.setCursor(5,55);
display.println("T:"+String(temperature)+"C"+" H:"+String(humi)+"%");
display.display();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// Evenement du Websocket server
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("[WSc] Disconnected!\n");
            if (webSocket.connectedClients()==0)
              digitalWrite(LED_Client,LOW);
              Serial.print("pas de clients connectés");
            break;
        case WStype_CONNECTED:
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            digitalWrite(LED_Client,HIGH);
            // send message to server when Connected
            webSocket.broadcastTXT("Connected");
            break;
  }
  String Donnees_recue = (const char *)payload;
  Serial.print("Données recue = ");
  Serial.println(Donnees_recue);

// Reception des données du client
  if(type == WStype_TEXT) {
    byte separator=Donnees_recue.indexOf('=');
    String var = Donnees_recue.substring(0,separator);
    Serial.print("var= ");
    Serial.println(var);
    String val = Donnees_recue.substring(separator+1);
    Serial.print("val= ");
    Serial.println(val);
    Serial.println(" ");

    if(var == "LEDonoff"){  
      if(val == "ON"){
        digitalWrite(LED_Statut,HIGH);
         valeur_LED_S ="ON";}
      else{
          digitalWrite(LED_Statut,LOW);
          valeur_LED_S="OFF";} 
    }
}
}

void setup() {
  Serial.begin(115200);

  pinMode(BP1, INPUT_PULLDOWN);
  pinMode(LED_Statut, OUTPUT);
  pinMode(LED_Client, OUTPUT);
  digitalWrite(LED_Statut, LOW);
  digitalWrite(LED_Client, LOW);

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
  request->send(LITTLEFS,"/index.css","text/css",false); 
  });
  
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(LITTLEFS,"/index.js","text/js",false); 
  });
   
  // Get request
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value(); 
      Serial.println("Get request:"+inputMessage);
      request->send(200, "text/html", String(temperature));
    }
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value(); 
      Serial.println("Get request:"+inputMessage);
      request->send(200, "text/html", String(humi));
    }    
    if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value(); 
      Serial.println("Get request:"+inputMessage);
      request->send(200, "text/html", String(valeur_bp));
    }   
  });
 
 // Post request
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
       message = request->getParam(PARAM_MESSAGE)->value();
      }  
    else {message = "No message sent";}
    request->send(200, "text/plain", "Hello, POST: " + message);
    });
 
    server.onNotFound(notFound);
    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D Pour 128x64
      Serial.println(F("SSD1306 allocation failed"));
    }
   
}
 
void loop() {
   webSocket.loop();
   humi = dht.readHumidity();
   temperature = dht.readTemperature();
   affichage_oled();
   JSONtxt = "{\"humi\":\""+String(humi)+"\",\"temperature\":\""+String(temperature)+"\",\"valeur_bp\":\""+valeur_bp+"\"}";
   webSocket.broadcastTXT(JSONtxt);
   delay(500);
}