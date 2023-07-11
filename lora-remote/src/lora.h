#pragma once
#include "heltec.h"
#include "config.h"
#include "ArduinoJson.h"
#include "chipID.h"
#include "helper.h"

#if defined(HAB_SYSTEM)
#include "hab.h"
#include <geolocate.h>
#endif

#if defined(ALERT_SYSTEM)
#include "warningled.h"
#endif

typedef struct {
    const char* topic;
    int hopCount;
} topicPriority;

void setupLora();
void onReceive(int packetSize);
void sendMessage(const char* outgoing);
void sendMessage(const char* topic, const char* payload);
void sendMessage(const char* topic, const char* hops, const char* payload, uint32_t packetID);
void sendPing();
void parsePacket(const char* loraMessage);
void relayPacket(char* topic, char* hopsData, char* data, uint32_t packetID);