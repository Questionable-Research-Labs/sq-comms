#pragma once

#ifdef ALERT_SYSTEM

#include <ArduinoJson.h>
#include <lora.h>
#include <chipID.h>
#include "warningled.h"

void alertSetup();
void sendAlert();

#endif