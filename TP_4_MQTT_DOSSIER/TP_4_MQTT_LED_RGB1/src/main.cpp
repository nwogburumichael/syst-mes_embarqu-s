#include <Arduino.h>
#include <AdafruitIO_WiFi.h>
 #include "FS.h"
#define IO_USERNAME  "Michael_Nwg"
#define IO_KEY    "aio_ZAiA33WokaeEnFuXlSFNpEmZSRQy"
#define WIFI_SSID  "mike"
#define WIFI_PASS  "nwogburu234"
#define LED_PIN 2 


// default PWM pins for ESP8266.
// you should change these to match PWM pins on other platforms.
#define RED_PIN   4
#define GREEN_PIN 5
#define BLUE_PIN  2
AdafruitIO_WiFi io (IO_USERNAME,IO_KEY,WIFI_SSID, WIFI_PASS);

// set up the  feed
AdafruitIO_Feed *RGB = io.feed("RGB");
AdafruitIO_Feed *LED = io.feed("LED");
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
  // put your main code here, to run repeatedly:

  // this function is called whenever a 'color' message
// is received from Adafruit IO. it was attached to
// the color feed in the setup() function above.

}

