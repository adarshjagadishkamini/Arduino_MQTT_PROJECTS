
#include "WiFi.h"
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"

#define DHTPIN 4     // esp8266 D2 pin map as 4 in Arduino IDE
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);


const char* ssid = "phone"; //choose your wireless ssid
const char* password =  "Jkaa1234"; //put your wireless password here.
const char mqtt_server[] = "192.168.142.236";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  //pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }
  
  dht.begin();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);

}

int timeSinceLastRead = 0;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  char output[80];
  
  if(timeSinceLastRead > 2000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
   client.publish("/lab/sensor", (float)h);
  delay(2000);
  
timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}
