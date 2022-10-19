#include <Arduino.h>
#include <AdafruitIO_WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "FS.h"

#define IO_USERNAME  "TimTq"
#define IO_KEY       "aio_femc009g6FIiRQkgjeEqsj55IhOu"
#define WIFI_SSID  "LAPTOP_T"
#define WIFI_PASS  "TIMON123"
#define LED_PIN 2
#define  DATA_PIN  26 
AdafruitIO_WiFi io (IO_USERNAME,IO_KEY,WIFI_SSID, WIFI_PASS);
DHT_Unified dht(DATA_PIN, DHT11);

// default PWM pins for ESP8266.
// you should change these to match PWM pins on other platforms.
#define RED_PIN   4
#define GREEN_PIN 5
#define BLUE_PIN  2
#define LDR 39

// photocell state
int current = 0;
int last = -1;


// set up the  feed
AdafruitIO_Feed *RGB = io.feed("RGB");
AdafruitIO_Feed *LED = io.feed("LED");
AdafruitIO_Feed *Temperature = io.feed("temperature");
AdafruitIO_Feed *Humidite = io.feed("humidite");
AdafruitIO_Feed *luminosite = io.feed("luminosité");
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  // write the current state to the led
  digitalWrite(LED_PIN, data->toPinLevel());
    // print RGB values and hex value
  Serial.println("Received:");
  Serial.print("  - R: ");
  Serial.println(data->toRed());
  Serial.print("  - G: ");
  Serial.println(data->toGreen());
  Serial.print("  - B: ");
  Serial.println(data->toBlue());
  Serial.print("  - HEX: ");
  Serial.println(data->value());

  // invert RGB values for common anode LEDs
  analogWrite(RED_PIN, 255 - data->toRed());
  analogWrite(GREEN_PIN, 255 - data->toGreen());
  analogWrite(BLUE_PIN, 255 - data->toBlue());

}
void setup() {

  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while(! Serial);
  dht.begin();
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Actviation de la fonction en fontion des données du dashboard
  LED->onMessage(handleMessage);
  RGB->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());

  // recois les valeur du Dashboard
  LED->get();
  RGB->get();
}

void loop() {
  io.run();
  current = analogRead(LDR);
  // return if the value hasn't changed
  if(current == last)
    return;

  Serial.print("sending -> ");
  Serial.println(current);
  luminosite->save(current);
  last = current;

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");
  Temperature->save(celsius);
  dht.humidity().getEvent(&event);

  Serial.print("humidité: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");

  Serial.print("LDR: ");
  Serial.println(analogRead(LDR));

  // save humidity to Adafruit IO
  Humidite->save(event.relative_humidity);

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(5000);

}
  // put your main code here, to run repeatedly:
