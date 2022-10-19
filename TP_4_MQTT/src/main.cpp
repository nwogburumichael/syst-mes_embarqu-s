#include <Arduino.h>
#include <AdafruitIO_WiFi.h>
#define IO_USERNAME  "TimTq"
#define IO_KEY       "aio_RyoP06Fgpg7xJAiV3erjFg3pyPkR"
#define WIFI_SSID  "LAPTOP_T"
#define WIFI_PASS  "TIMON123"
#define LED_PIN 2 
AdafruitIO_WiFi io (IO_USERNAME,IO_KEY,WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *LED = io.feed("LED");
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  // write the current state to the led
  digitalWrite(LED_PIN, data->toPinLevel());

}
void setup() {

  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  LED->onMessage(handleMessage);
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());

  LED->get();
}

  


void loop() {
  io.run();
  // put your main code here, to run repeatedly:
}