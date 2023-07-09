#pragma once

#ifdef ALERT_SYSTEM

#include <ArduinoJson.h>
#include <lora.h>
#include "heltec.h"
#include "warningled.h"

void alertSetup();
void sendAlert();

#endif