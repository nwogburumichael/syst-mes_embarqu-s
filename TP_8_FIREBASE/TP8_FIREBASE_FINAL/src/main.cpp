
#include <Arduino.h>
#if defined(ESP32)
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

//Oled 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Insert your network credentials
#define WIFI_SSID "LAPTOP_T"
#define WIFI_PASSWORD "TIMON123"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBLuTCDpuHVK3jvKWU9Ia4DXOJ9oDd0KQE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://tp8-firebase-5d8b5-default-rtdb.europe-west1.firebasedatabase.app/" 

#define led1 14
#define led2 13
#define bp 12
#define DHTpin 26
#define LDR 34
DHT dht(DHTpin, DHT11);
//variable ldr_dht
float celsius;
long Lumi;
float Humi;


//Variable propre à la classe FireBase
FirebaseData stream;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String valeurLed ="";
String valeurBp = "";


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
volatile bool dataChanged = false;

//fonction états LEDS via Firebase
void actualisation(String chemin,String valeur){

 //prise de la valeur depuis la database
  if(String(chemin) == "/json/led1" && String(valeur) == "on"){
    Serial.printf("LED1 allumé %s",valeur);
    digitalWrite(led1,HIGH);
  }
  else if(String(chemin) == "/json/led1" && String(valeur) == "off"){
     Serial.printf("LED1 eteint%s",valeur);
    digitalWrite(led1,LOW);
  }

  if(String(chemin) == "/json/led2" && String(valeur) == "on"){
    Serial.printf("LED2 allumé %s",valeur);
    digitalWrite(led2,HIGH);
  }
  else if(String(chemin) == "/json/led2" && String(valeur) == "off"){
     Serial.printf("LED2 eteint%s",valeur);
    digitalWrite(led2,LOW);
  }
}
// fonction Callback
void streamCallback(FirebaseStream data){
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); 
  Serial.println();

  //appel de la fonction d'actualisation des etat des LEDS en apssant sur Firebase
  actualisation(data.dataPath(),data.to<String>());


  // This is the size of stream payload received (current and max value)
  // Max payload size is the payload size under the stream path since the stream connected
  // and read once and will not update until stream reconnection takes place.
  // This max value will be zero as no payload received in case of ESP8266 which
  // BearSSL reserved Rx buffer size is less than the actual stream payload.
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());

  // Due to limited of stack memory, do not perform any task that used large memory here especially starting connect to server.
  // Just set this flag and check it status later.
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}
//prise de temp/hum/lumi
void Temp_HUM_LDR(){
 
  celsius = dht.readTemperature();
  Humi = dht.readHumidity();
  //esp delivre 12 Bytes par defaut
  Lumi= map(analogRead(LDR), 0, 4095, 0, 100);
}
//fonction affichage oled
void affichage_oled(){

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(1, 5);
display.println("Led1 : " + String(digitalRead(led1)));
display.setCursor(1,15);
display.println("Led2 : "+ String(digitalRead(led2)));
display.setCursor(1,25);
display.println("SW : "+ String(digitalRead(bp)));
display.setCursor(1,35);
display.println("T: "+String(celsius)+" C "+" H: "+String(Humi)+" %");
display.setCursor(1,45);
display.println("Luminosite : " + String(Lumi)+" %");
display.display();
}

void setup(){
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(bp,INPUT_PULLDOWN);
  pinMode(LDR,INPUT);
  dht.begin();
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //partie oled
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D Pour 128x64
      Serial.println(F("SSD1306 allocation failed"));
    }

  //Assigner la cle API
  config.api_key = API_KEY;

  //Assigner l url
  config.database_url = DATABASE_URL;

  //Sign up
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  //Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; 
  
  if (!Firebase.RTDB.beginStream(&stream, "/Signalisation/stream/data"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

     
  //Envoye de l'etat des LEDS à Firebase
  FirebaseJson json;
  json.add("led1", "Off");
  json.add("led2", "Off");
  Serial.printf("Set json... %s\n\n", Firebase.RTDB.setJSON(&fbdo, "/Signalisation/stream/data/json", &json) ? "ok" : fbdo.errorReason().c_str());
    
}

void loop(){
  Temp_HUM_LDR();

  if (digitalRead(bp) == true){
    valeurBp = "On";
  }

  else if (digitalRead(bp) == false){
    valeurBp = "Off";
  }
  affichage_oled();
  //Condition pour envoyer nos donnees sur Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    }
    //envoie de temp_humi_lumi a firebase 
    FirebaseJson json;
    json.add("Temperature", celsius);
    json.add("Humidité", Humi);
    json.add("Luminosité", Lumi);
    Serial.printf("Set json... %s\n\n", Firebase.RTDB.pushJSON(&fbdo, "/test/stream/data/json", &json) ? "ok" : fbdo.errorReason().c_str());

    //verification que l'etat du bouton s'est envoye
    if (Firebase.RTDB.setString(&fbdo, "Bouton/String", valeurBp)){

    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " +fbdo.errorReason());
    }
    //prise de la valeur du bouton 
    if (Firebase.RTDB.getString(&fbdo, "/Bouton/String")) {
      if (fbdo.dataType() == "String") {
        String Valed = fbdo.stringData();
        Serial.println(Valed);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    } 
  

    
  if (dataChanged)
  {
    dataChanged = false;
    
  }
  

}
