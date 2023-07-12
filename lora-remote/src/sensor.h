#pragma once

#ifdef SENSOR_SYSTEM

#include <ArduinoJson.h>
#include <lora.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <chipID.h>
#include <SoftwareSerial.h>

void sensorSetup();
void sensorLoop();

#endif