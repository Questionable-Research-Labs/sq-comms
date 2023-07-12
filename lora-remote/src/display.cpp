#include <display.h>

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, GEOMETRY_128_64);

void initDisplay() {
    pinMode(RST_OLED,OUTPUT);
    digitalWrite(RST_OLED, LOW);
    delay(50);
    digitalWrite(RST_OLED, HIGH);

    bool status = display.init();
    if (status) {
        Serial.println("Display init success");
    } else {
        Serial.println("Display init failed");
        return;
    }
    
    display.flipScreenVertically();
    display.clear();
    // display.invertDisplay();
    display.drawString(0, 0, "Display init success!");
    display.drawString(0, 10, "Waiting for initialising...");
    display.display();
}

uint32_t lastLoRaPacketTime = 0;
float lastLoRaPacketRssi = 0;


void displayUpdateLoraStats(float rssi) {
    lastLoRaPacketRssi = rssi;
    lastLoRaPacketTime = millis();
}

uint32_t lastDisplayUpdate = 0;

void renderState() {
    if (millis() - lastDisplayUpdate < 500) {
        return;
    }

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Chip ID: " + String(chipID));
    
    #if defined(HAB_SYSTEM)
    display.drawString(0, 10, "HAB Module");
    #elif defined(SENSOR_SYSTEM)
    display.drawString(0, 10, "Sensor Module");
    #elif defined(REMOTE_SYSTEM)
    display.drawString(0, 10, "Remote Module");
    char loraStats[22] = "";
    float secondsSinceLastPacket = (float)(millis() - lastLoRaPacketTime) / 1000;
    sprintf(loraStats, "TSU %0.1fs, RSSI: %.0f", secondsSinceLastPacket, lastLoRaPacketRssi);
    display.drawString(0, 20, loraStats);

    #endif

    display.display();
    lastDisplayUpdate = millis();
}