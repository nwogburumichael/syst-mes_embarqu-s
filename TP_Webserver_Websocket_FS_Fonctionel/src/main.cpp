#include <Arduino.h>

#include "FS.h"
#include <LittleFS.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>


 
const char* ssid = "Wifi-Abosi";
const char* password = "Nwogburu234";
const char* PARAM_MESSAGE = "message";
const char* filename = "index.html";


// Default Threshold Temperature Value
String inputMessage = "25.0";
String lastTemperature = "96";
String enableArmChecked = "checked";
String inputMessage2 = "true";

const char* PARAM_INPUT_1 = "threshold_input";
const char* PARAM_INPUT_2 = "enable_arm_input";
 
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

/*void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server");
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
 
  }
}*/
bool LEDonoff = false;
int NB = 0;
String JSONtxt ="";


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// Evenement du Websocket server
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  String Donnees_recue = (const char *)payload;
  Serial.print("Données recue = ");
  Serial.println(Donnees_recue);

  if(type == WStype_TEXT) //receive text from client
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

    // initialisation des sortie
 

    // Connexion Wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
 
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Implémentation de litlle FS
   if(!LITTLEFS.begin(true)){
    Serial.println("An Error has occurred while mounting LITTLEFS");
    }
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LITTLEFS, "/index.html","text/html",false); 
    });

  // Receive an HTTP GET request at <ESP_IP>/get?threshold_input=<inputMessage>&enable_arm_input=<inputMessage2>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
        enableArmChecked = "checked";
      }
      else {
        inputMessage2 = "false";
        enableArmChecked = "";
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
   NB += 1;
   delay(2000);
   JSONtxt = "{\"TEMP\":\""+String(NB)+"\"}";
   webSocket.broadcastTXT(JSONtxt);
 
}