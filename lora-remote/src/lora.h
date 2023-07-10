#pragma once
#include "heltec.h"
#include "config.h"
#include "ArduinoJson.h"
#include "chipID.h"

#if defined(HAB_SYSTEM)
#include "hab.h"
#endif

#if defined(ALERT_SYSTEM)
#include "warningled.h"
#endif

void setupLora();
void onReceive(int packetSize);
void sendMessage(char* outgoing);
void sendMessage(char* topic, char* payload);
void sendMessage(char* topic, char* hops, char* payload);
void sendPing();
void parsePacket(const char* loraMessage);