#pragma once

#include <ArduinoJson.h>
#include <lora.h>
#include <chipID.h>
#include "heltec.h"
#include "warningled.h"

void alertSetup();
void sendAlert();
