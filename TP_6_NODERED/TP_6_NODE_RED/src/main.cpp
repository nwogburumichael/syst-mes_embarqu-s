
#include <WifiClient.h>
#include <PubSubClient.h>
#include <Wifi.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>
//pin Variables

#define PINLED 2
#define DHTpin 26
#define LDR 39
#define RGB_PIN 27
DHT dht(DHTpin, DHT11);

//Var RGB
int Red = 0;
int Green = 0;
int Blue = 0 ;
// creation rgb
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, RGB_PIN, NEO_GRB + NEO_KHZ800);
// JSON envoy
String output;
DynamicJsonDocument doc(96);

//Json reception

DynamicJsonDocument doc1(96);


const char* ssid = "LAPTOP_T";
const char* password = "TIMON123";
const char* mqtt_server = "192.168.1.51";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
    
  }
  Serial.println();
  Serial.println(messageTemp);
  if (String(topic) == "inTopic") {

    DeserializationError error = deserializeJson(doc1, messageTemp);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}
    //stockage des variables du json
    Red = doc1["red"];
    Green = doc1["green"];
    Blue = doc1["blue"];

    for (int i = 0; i < strip.numPixels(); i++)
        {
        strip.setPixelColor(i, strip.Color(Red, Green, Blue));
        }
        strip.show();

      Serial.print(Red + Green + Blue );
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(PINLED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  dht.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //condition pour envoyer les données
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    float f = dht.readTemperature();
    float h = dht.readHumidity();
    int Vldr = analogRead(LDR);   //lecture de la luminosité
      
      
      //String(f)
      doc["TEMPE"] = String(f);
      doc["HUMI"] = String(h);
      doc["LUMI"] = String(Vldr);   //envoi de la valeur de la luminosité
      String payload;  
      serializeJson(doc, payload);

      client.publish("info", payload.c_str()); //envoi de données sous format string
      Serial.println();
      Serial.print("Données envoyées");

      Serial.println(payload);
    
    
 
    
  }
}