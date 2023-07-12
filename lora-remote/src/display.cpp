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

bool remoteAlert = false;
char remoteAlertMsg[22] = "";
bool localAlert = false;

void displayRemoteAlert(bool alerting, const char * msg) {
    if (strlen(msg) > 21) {
        Serial.println("Message too long for display!");
    }
    memccpy(remoteAlertMsg, msg, 0, 21);
    remoteAlertMsg[21] = '\0';
    
    remoteAlert = alerting;
}

void displayLocalAlert(bool alerting) {
    localAlert = alerting;
}

uint32_t lastDisplayUpdate = 0;

void renderState() {
    if (millis() - lastDisplayUpdate < 500) {
        return;
    }

    display.clear();
    display.normalDisplay();
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

    if (remoteAlert || localAlert) {
        display.setFont(ArialMT_Plain_16);
        display.drawString(0, 30, "ALERT");
        display.setFont(ArialMT_Plain_10);

        // Invert display every two seconds;
        if (millis() / 1000 % 2) {
            display.invertDisplay();
        } else {
            display.normalDisplay();
        }

        if (remoteAlert) {
            display.drawString(0, 46, remoteAlertMsg);
        }
        else if (localAlert) {
            display.drawString(0, 46, "BROADCASTING");
        }
    }

    #endif

    display.display();
    lastDisplayUpdate = millis();
}