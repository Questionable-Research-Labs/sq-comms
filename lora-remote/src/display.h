#pragma once
#include "Arduino.h"
#include <Wire.h>
// #include "SSD1306Wire.h"
#include <SSD1306Wire.h>
#include <chipID.h>

void initDisplay();
void renderState();
void displayUpdateLoraStats(float rssi);
void displayLocalAlert(bool alerting);
void displayRemoteAlert(bool alerting, const char * msg);