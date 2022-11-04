
#include <AdafruitIO_WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>

#define IO_USERNAME "Michael_Nwg"
#define IO_KEY     "aio_MiTf37vC7EtaAPboALYoJyncahkg"
#define WIFI_SSID  "Wifi-Abosi"
#define WIFI_PASS  "Nwogburu234"



#define RED_PIN   4
#define GREEN_PIN 5
#define BLUE_PIN  2
#define LDR 39
#define RGB_PIN 0
#define LED_PIN 2
#define DATA_PIN  26 
#define Push_Button1 12

// RGB Var
int couleurRed = 0;
int couleurBlue = 0;
int couleurGreen = 0;

// LDR
int LDR_Value= 0;


AdafruitIO_WiFi io (IO_USERNAME,IO_KEY,WIFI_SSID, WIFI_PASS);
DHT_Unified dht(DATA_PIN, DHT11);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, RGB_PIN, NEO_GRB + NEO_KHZ800);

// set up the  feed
AdafruitIO_Feed *RGB_BLUE = io.feed("RGB_BLUE");
AdafruitIO_Feed *RGB_RED = io.feed("RGB_RED");
AdafruitIO_Feed *RGB_GREEN = io.feed("RGB_GREEN");
AdafruitIO_Feed *LED = io.feed("LED");
AdafruitIO_Feed *Temperature = io.feed("temperature");
AdafruitIO_Feed *Humidite = io.feed("humidite");
AdafruitIO_Feed *luminosite = io.feed("luminosité");
AdafruitIO_Feed *TXT_BOX = io.feed("TXT_BOX");

// Fonction de reception des données //
void handle_LED(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH){
    Serial.println("HIGH");
    TXT_BOX->save("Led: ON");
    }
    
  else{
    Serial.println("LOW");
    TXT_BOX->save("LED: OFF");
  }

  // write the current state to the led
  digitalWrite(LED_PIN, data->toPinLevel());
}

void handleRGB_BLUE(AdafruitIO_Data *data){
  couleurBlue = data->toInt();
  // print RGB value
  Serial.print("B: <-" );
  Serial.println(couleurBlue);
  // RGB Neopixel
  for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, strip.Color(couleurRed, couleurGreen, couleurBlue));
    }
    strip.show();
}

void handleRGB_GREEN(AdafruitIO_Data *data){
  couleurGreen = data->toInt();
  // print RGB value
  Serial.print("G: <-");
  Serial.println(couleurGreen);
  // RGB Neopixel
  for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, strip.Color(couleurRed, couleurGreen, couleurBlue));
    }
    strip.show();
}

void handleRGB_RED(AdafruitIO_Data *data){
  couleurRed = data->toInt();
  // print RGB value
  Serial.print("R: <-");
  Serial.println(couleurRed);
  // RGB Neopixel
  for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, strip.Color(couleurRed, couleurGreen, couleurBlue));
    }
    strip.show();
}
// Fonction d'actualisation des données
void Actualisation(){

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float celsius = event.temperature; // ver
  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");
  Temperature->save(celsius);

  dht.humidity().getEvent(&event);
  Serial.print("humidité: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");
  Humidite->save(event.relative_humidity);

  LDR_Value = analogRead(LDR);
  Serial.print("Luminosité -> ");
  Serial.println(LDR_Value);
  luminosite->save(LDR_Value);
  }

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(Push_Button1, INPUT_PULLDOWN);
  Serial.begin(115200);
  while(!Serial);
  dht.begin();
  Serial.print("Connecting to Adafruit IO");
  io.connect();


  // Création de message CALLBACK
  LED->onMessage(handle_LED);
  RGB_BLUE->onMessage(handleRGB_BLUE);
  RGB_RED->onMessage(handleRGB_RED);
  RGB_GREEN->onMessage(handleRGB_GREEN);


  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());

 
  LED->get();
  RGB_BLUE->get();
  RGB_RED->get();
  RGB_GREEN->get();
}

void loop() {
  io.run();
  if (digitalRead(Push_Button1) == HIGH){
    Actualisation();
  }
}

