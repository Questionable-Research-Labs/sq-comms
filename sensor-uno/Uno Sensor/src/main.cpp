#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "Adafruit_CCS811.h"
#include <SPI.h>
#include <SoftwareSerial.h> // Using this to communicate to the other board
#include <ArduinoJson.h>

Adafruit_CCS811 ccs;
#define DHTPIN 2
#define DHTTYPE DHT22

SoftwareSerial otherBoard(10, 11); // RX, TX
uint32_t delayMS;

int dustPin = 0;
int dustLED = 7;
int samplingTime = 280;
int deltaTime = 40;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int dust = 0;
int hum = 0;
int temp = 0;

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(dustLED, OUTPUT);

  dht.begin();
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000;
  Serial.begin(9600);
  otherBoard.begin(9600);

}

void loop() {
  // Dust Sensor ====================
  digitalWrite(dustLED, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(dustPin);
  delayMicroseconds(deltaTime);
  digitalWrite(dustLED, HIGH);
  delayMicroseconds(samplingTime);
  calcVoltage = voMeasured * (5.0 / 1024);
  dustDensity = 170 * calcVoltage - 0.1;
  Serial.println(dustDensity); // unit: ug/m3

  // DH22 SENSOR ====================
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  

  temp = event.temperature;
  

  if (isnan(event.temperature)) { 
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C");
  }

  dht.humidity().getEvent(&event);
  hum = event.relative_humidity;

  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println("%");
  }

  // PREPARING TO SEND
  StaticJsonDocument<200> doc;
  doc["dust"] = dustDensity;
  doc["temp"] = temp;
  doc["hum"] = hum;

  String output;
  serializeJson(doc, output);
  Serial.println(output);

  otherBoard.println(output);
  delay(2000);
}
