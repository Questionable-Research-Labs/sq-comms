#pragma once

#if defined(HAB_SYSTEM)
#include <FS.h>
#include <SPIFFS.h>
#include <PubSubClient.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager
#include <config.h>
#include <ArduinoJson.h>

void setupHab();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void loopHab();
void forwardPacket(const char *topic, const char *payload);
void processPingPacket(const char *payload, float rssi);
void reconnect();
void saveConfig();
void saveConfigCallback();
int loadConfig();
#endif