#pragma once
#include "heltec.h"
#include "config.h"

#if defined(HAB_SYSTEM)
#include "hab.h"
#endif

#if defined(ALERT_SYSTEM)
#include "warningled.h"
#endif

void setupLora();
void onReceive(int packetSize);
void sendRawMessage(char* outgoing);
void sendMessage(char* topic, char* payload);