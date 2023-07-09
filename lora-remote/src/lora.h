#pragma once
#include "heltec.h"
#include "config.h"

void setupLora();
void onReceive(int packetSize);
void sendRawMessage(char* outgoing);
void sendMessage(char* topic, char* payload);