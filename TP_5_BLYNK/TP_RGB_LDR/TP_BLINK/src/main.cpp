
#include <Adafruit_NeoPixel.h>

#define BLYNK_TEMPLATE_ID           "TMPLdgNvmLUn"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "BKj7d7LNUjBounCH0wzMH4x6aCAIWfgs"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
#define LDR 39
#define LED_PWM 2
#define RGB_PIN 0

// var couleur RGB
int couleur1 = 0;
int couleur2 = 0;
int couleur3 = 0;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "mike";
char pass[] = "nwogburu234";

BlynkTimer timer;
int compteur = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, RGB_PIN, NEO_GRB + NEO_KHZ800);


BLYNK_WRITE(V4)
{
  couleur1 = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    
    strip.setPixelColor(i, strip.Color(couleur1, couleur2, couleur3));
  }
  strip.show();
}

BLYNK_WRITE(V6)
{
  couleur2 = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    
    strip.setPixelColor(i, strip.Color(couleur1, couleur2, couleur3));
  }
  strip.show();
}

BLYNK_WRITE(V5)
{
  couleur3 = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    
    strip.setPixelColor(i, strip.Color(couleur1, couleur2, couleur3));
  }
  strip.show();
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)

    {
      // Set incoming value from pin V0 to a variable
      int value = param.asInt();
      Serial.println("Valeur bp"+String(value));

      // Update state
      Blynk.virtualWrite(V1, value);
      //digitalWrite(1,value); // allumer la led
    }
/*
// fonction pour la led avec singeaux PWM
BLYNK_WRITE(V4)
    {
      // Set incoming value from pin V0 to a variable
      int value1 = param.asInt();
      Serial.println("Valeur LED"+ String(value1));

      // Update state
      analogWrite(LED_PWM,value1); // Ecrire cette valeur dans une sortie PWM
    }

*/
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
  //Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V2,analogRead(LDR));
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial.print("probleme");
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
  compteur++ ;
  Blynk.run();
  timer.run();

}



