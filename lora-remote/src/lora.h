#pragma once
#include "heltec.h"

void setupLora();
void sendMessage(String outgoing);
void onReceive(int packetSize);