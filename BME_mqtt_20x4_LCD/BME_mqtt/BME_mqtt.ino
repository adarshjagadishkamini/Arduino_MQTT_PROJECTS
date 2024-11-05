#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
//#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 
Adafruit_BME680 bme;

int _moisture,sensor_analog;
const int sensor_pin = A0;

const char* ssid = "ABDULLAH DESKTOP";
const char* password = "bilal@2018";
const char* mqtt_server = "192.168.137.249";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  lcd.init();
  lcd.clear();
lcd.backlight();
  Serial.begin(115200);
  while (!Serial);
  pinMode(15,INPUT);
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  
  lcd.print("WiFi connected to");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  delay(3000);
  lcd.clear();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, 1883);
}

void loop() {
//lcd.clear();

  if (!client.connected()) {
    reconnect();
  }

  sensor_analog = analogRead(sensor_pin);
  _moisture = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  
  
  StaticJsonDocument<80> doc;
  char output[80];
 
  
    float temp = bme.readTemperature();
    float pressure = bme.readPressure()/100.0;
    float humidity = bme.readHumidity();
    float gas = bme.readGas()/1000.0;
    doc["t"] = temp;
    doc["p"] = pressure;
    doc["h"] = humidity;
    doc["g"] = gas;
    doc["m"] = _moisture;
    serializeJson(doc, output);
    Serial.println(output);
    client.publish("/lab/sensors", output);
    lcd.print("Temp:");
    lcd.setCursor(10,0);
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print("Pressure:");
    lcd.setCursor(10,1);
    lcd.print(pressure);
    lcd.setCursor(0,2);
    lcd.print("Humidity:");
    lcd.setCursor(10,2);
    lcd.print(humidity);
    lcd.setCursor(0,3);
    lcd.print("Moisture:");
    lcd.setCursor(10,3);
    lcd.print(_moisture);
    
    
   delay(500);
    
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
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}
