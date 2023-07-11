#pragma once
#if defined(HAB_SYSTEM)

#include <Arduino.h>
#include <ArduinoJson.h>
#include <config.h>
#include <chipID.h>
#include <hab.h>

typedef struct {
    const char* chipID;
    int rssi;
} alertNode;

typedef struct {
    const char* chipID;
    uint32_t packetID;
    alertNode rssis[32];
    int numRssis;
} alertSet;

void estimateDistances(alertSet alert);
void newAlert(const char* alteredChipID, uint32_t packetID, alertNode firstHopNode);
void processAlert(const char* hops, const char* payload, uint32_t packetID, int rssi);

#endif