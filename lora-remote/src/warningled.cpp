#include <warningled.h>

#ifdef ALERT_SYSTEM

#define LED_RED 12
#define LED_GREEN 25
#define LED_BLUE 21

void setupLED() {
    Serial.println("Setting up warning LED");
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

void warningLED(bool red, bool green, bool blue) {
    digitalWrite(LED_RED, red);
    digitalWrite(LED_GREEN, green);
    digitalWrite(LED_BLUE, blue);
}

void forwardLED(const char *topic, const char *payload) {
    if (strcmp(topic, "ALERT") == 0) {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        bool active = doc["active"];
        if (active) {
            warningLED(true, false, false);
        }
        else {
            warningLED(false, false, false);
        }
    }
}

#endif
