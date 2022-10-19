#include <Arduino.h>
#include <AdafruitIO_WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define IO_USERNAME  "TimTq"
#define IO_KEY       "aio_SsWQ822D79Fn0tFejz8FOshjFazP"
#define WIFI_SSID  "LAPTOP_T"
#define WIFI_PASS  "TIMON123"
#define LED_PIN 2
#define  DATA_PIN  26 
AdafruitIO_WiFi io (IO_USERNAME,IO_KEY,WIFI_SSID, WIFI_PASS);
DHT_Unified dht(DATA_PIN, DHT11);

AdafruitIO_Feed *LED = io.feed("LED");
AdafruitIO_Feed *Temperature = io.feed("temperature");
AdafruitIO_Feed *Humidite = io.feed("humidite");
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
  dht.begin();
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

  // save humidity to Adafruit IO
  Humidite->save(event.relative_humidity);

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(5000);

}
  // put your main code here, to run repeatedly:
