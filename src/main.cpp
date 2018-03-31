
/* Iot Device Park-Me using Alora sensor Kit
   main.cpp
   Created  by Mukmin on 26/08/2017
   Copyright © 2017 Mukmin. All rights reserved.
*/


#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Wire.h>
#include <AloraSensorKit.h>

AloraSensorKit sensorKit;

const char* ssid = "ssid-wifi";
const char* password =  "pass-wifi";
const char* mqttServer = "serverName"; 
const int mqttPort = 1883;
const char* mqttUser = "admintes"; 
const char* mqttPassword = "admin123"; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  sensorKit.begin();
  pinMode(14, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

  //client.publish("esp/test", "Hello from ESP32");

}

void loop() {
  //client.loop();
  sensorKit.run();

  SensorValues sensorData = sensorKit.getLastSensorData();

  Serial.println("Magnet: ");
  float dataSensor = sensorData.magZ;
  Serial.println(dataSensor);

  float compareValueFloat= dataSensor * -100;
  int compareValue = round(compareValueFloat);
  Serial.println(compareValue);

  if (compareValue >= 45) {
    digitalWrite(14,LOW);
    Serial.println("Mobil 1");
    client.publish("mobil/data", "1"); //publish data 1  to topic "mobil/data"
  } else {
    digitalWrite(14,HIGH);
    Serial.println("Mobil 0");
    client.publish("mobil/data", "0"); //publish data 0 topic "mobil/data"
  }
  delay(1000);
}
