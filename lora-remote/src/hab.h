#pragma once

#if defined(HAB_SYSTEM)

#include <PubSubClient.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager
#include <config.h>

void setupHab();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void loopHab();
void forwardPacket(const char *topic, const char *payload);

#endif