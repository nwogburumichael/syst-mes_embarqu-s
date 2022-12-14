// Données de connexion au serveur blynk
#define BLYNK_TEMPLATE_ID "TMPLdgNvmLUn"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "BKj7d7LNUjBounCH0wzMH4x6aCAIWfgs"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "FS.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
char auth[] = BLYNK_AUTH_TOKEN;

#define DHTpin 26
#define LDR 33
#define RGB_PIN 27
#define LED_Blynk 32

// var couleur RGB
int Red = 0;
int Green = 0;
int Blue = 0;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wifi-Abosi";
char pass[] = "Nwogburu234";

BlynkTimer timer;
DHT dht(DHTpin, DHT22);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, RGB_PIN, NEO_GRB + NEO_KHZ800);

int compteur = 0;

BLYNK_WRITE(V4)
{
  Red = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(Red, Green, Blue));
  }
  strip.show();
}

BLYNK_WRITE(V6)
{
  Green = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(Red, Green, Blue));
  }
  strip.show();
}

BLYNK_WRITE(V5)
{
  Blue = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(Red, Green, Blue));
  }
  strip.show();
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
  {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  digitalWrite(LED_Blynk,value);
  Blynk.virtualWrite(V9, value);
  }

void Actualisation(){
  Blynk.virtualWrite(V2,analogRead(LDR));
  float celsius = dht.readTemperature();
  float humi = dht.readHumidity();
  Blynk.virtualWrite(V7, celsius);
  Blynk.virtualWrite(V8, humi);   
}


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
  Actualisation();  
}

void setup()
{
  pinMode(LED_Blynk, OUTPUT);
  pinMode(LDR,INPUT);
  digitalWrite(LED_Blynk, LOW);
  // Debug console
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  strip.begin();
  strip.show();
}

void loop()
{ 
  Blynk.run();
  timer.run();
}



