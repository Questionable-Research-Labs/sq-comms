#pragma once
#include "config.h"
#include "ArduinoJson.h"
#include "chipID.h"
#include "helper.h"
#include <RadioLib.h>

#if defined(OUTPUT_NEOPIXEL)
#include "neolights.h"
#endif

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
void LoRaCheckForPacket();
void setNewMessageFlag();
int sendMessage(const char* outgoing);
void sendMessage(const char* topic, const char* payload);
void sendMessage(const char* topic, const char* hops, const char* payload, uint32_t packetID);
void sendPing();
void parsePacket(const char* loraMessage);
void relayPacket(char* topic, char* hopsData, char* data, uint32_t packetID);
void addMessageToOutQueue(const char* outgoing);
void proccessOutQueueCycle();