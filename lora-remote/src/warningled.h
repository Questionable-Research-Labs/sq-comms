#pragma once

#ifdef REMOTE_SYSTEM

#include <Arduino.h>
#include <ArduinoJson.h>
#include <display.h>

void setupLED();
void warningLED(bool red, bool green, bool blue);
void forwardLED(const char *topic, const char *payload);

#endif