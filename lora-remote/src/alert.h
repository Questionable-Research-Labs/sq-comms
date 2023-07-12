#pragma once

#ifdef ALERT_SYSTEM

#include <ArduinoJson.h>
#include <lora.h>
#include <chipID.h>
#include <display.h>

#if defined(REMOTE_SYSTEM)
#include "warningled.h"
#endif

void alertSetup();
void sendAlert();

#endif