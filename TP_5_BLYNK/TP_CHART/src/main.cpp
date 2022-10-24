#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPL8FoyKy4Y"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "8-kpiu3CLAaARM00sybjAmPet90DqhYu"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define LED_PIN 2
#define BP1 12
#define DHTpin 26
DHT dht(DHTpin, DHT11);

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
// var changement temp
float actuel = 0;
float dernier = -1;
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LAPTOP_T";
char pass[] = "TIMON123";
//WidgetLED led3(V5);
int btnState = HIGH;
BlynkTimer timer;

void Temp(){
 
  float celsius = dht.readTemperature();
  Blynk.virtualWrite(V6, celsius);
  Serial.print(celsius);
      
}

// This function is called every time the Virtual Pin 0 state changes
void myTimerEvent()
{
Temp();   
}


BLYNK_WRITE(V4)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  digitalWrite(LED_PIN, param.asInt());
}
void etatBP(){
if (digitalRead(BP1)==1){
Blynk.virtualWrite(V5,1);
}
  else{
    Blynk.virtualWrite(V5,0);
  }

}


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}


void setup()
{
  // Debug console
  pinMode(LED_PIN, OUTPUT);
  pinMode(BP1, INPUT_PULLDOWN);
  Serial.begin(115200);
  dht.begin();

  Blynk.begin(auth, ssid, pass);
  timer.setInterval ( 1000L , myTimerEvent);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
 
}

void loop()
{
  Blynk.run();
  etatBP();
  
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

