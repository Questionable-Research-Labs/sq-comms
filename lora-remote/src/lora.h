#pragma once
#include "heltec.h"
#include "config.h"
#include "ArduinoJson.h"
#include "chipID.h"

#if defined(HAB_SYSTEM)
#include "hab.h"
#endif

void setupLora();
void onReceive(int packetSize);
void sendRawMessage(char* outgoing);
void sendMessage(char* topic, char* payload);
void sendPing();