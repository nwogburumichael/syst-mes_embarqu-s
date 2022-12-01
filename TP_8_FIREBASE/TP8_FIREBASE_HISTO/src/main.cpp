#include <Arduino.h>
#if defined(ESP32)
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTpin 26
DHT dht(DHTpin, DHT11);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "LAPTOP_T"
#define WIFI_PASSWORD "TIMON123"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBLuTCDpuHVK3jvKWU9Ia4DXOJ9oDd0KQE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://tp8-firebase-5d8b5-default-rtdb.europe-west1.firebasedatabase.app/" 

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"
#define led 14
#define bp 12
#define LDR 39

//Define Firebase Data object
// Define Firebase Data object
FirebaseData stream;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String valeurLed ="";


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
volatile bool dataChanged = false;


void streamCallback(FirebaseStream data){
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); // see addons/RTDBHelper.h
  Serial.println();

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
void Temp_HUM_LDR(){
 
  float celsius = dht.readTemperature();
  float Humi = dht.readHumidity();
  
      
}

void setup(){
  pinMode(led,OUTPUT);
  pinMode(bp,INPUT);
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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  if (!Firebase.RTDB.beginStream(&stream, "/test/stream/data"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){

  float celsius = dht.readTemperature();
  float Humi = dht.readHumidity();
  float Lumi = (analogRead(LDR)/10.24);
  digitalWrite(led,random(0,3));

  if (digitalRead(led) == true){
    valeurLed = "On";
  }

  else if (digitalRead(led) == false){
    valeurLed = "Off";
  }


  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
   // test
    count++;
    FirebaseJson json;
    json.add("Temperature", celsius);
    json.add("Humidité", Humi);
    json.add("Luminosité", Lumi);
    Serial.printf("Set json... %s\n\n", Firebase.RTDB.pushJSON(&fbdo, "/test/stream/data/json", &json) ? "ok" : fbdo.errorReason().c_str());
  // test  
    
    
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.pushInt(&fbdo, "test/int", count)){
      // Serial.println("PASSED");
      // Serial.println("PATH: " + fbdo.dataPath());
      // Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " +fbdo.errorReason());
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.pushString(&fbdo, "test/String", valeurLed)){
      // Serial.println("PASSED");
      // Serial.println("PATH: " + fbdo.dataPath());
      // Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " +fbdo.errorReason());
    }
    
    if (Firebase.RTDB.getString(&fbdo, "/test/String")) {
      if (fbdo.dataType() == "String") {
        String Valed = fbdo.stringData();
        Serial.println(Valed);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    } 
  }

    // test
  if (dataChanged)
  {
    dataChanged = false;
    // When stream data is available, do anything here...
  }
  // test

}
