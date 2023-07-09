#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

void setupLED();
void warningLED(bool red, bool green, bool blue);
void forwardLED(const char *topic, const char *payload);
