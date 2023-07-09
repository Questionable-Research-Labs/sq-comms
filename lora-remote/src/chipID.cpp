#include <chipID.h>

char chipID[13] = "";

void setChipId() {
    uint64_t espMac = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
    sprintf(chipID, "%04X%08X", (uint16_t)(espMac >> 32), (uint32_t)espMac);
}