#include <alert.h>

#ifdef ALERT_SYSTEM

#define SWITCH_PIN 13

uint lastAlertMS = 0;

bool alertState = false;

void alertSetup() {
    Serial.println("Setting up alert system");
    pinMode(SWITCH_PIN, INPUT);
}

void sendAlert() {
    DynamicJsonDocument doc(1024);

    // query switch
    // if switch is on or toggled, send alert message, either true or false

    bool state = digitalRead(SWITCH_PIN);

    #if defined(HAB_SYSTEM)
    // Hab system is reverse
    state = !state;
    #endif

    if (state || alertState) {
        if ((millis() - lastAlertMS < 10000) && (state == alertState) ) {
            Serial.println("Alert already sent in last 10 seconds");
            return;
        }
        alertState = state;


        doc["from"] = chipID;
        #if defined(HAB_SYSTEM)
        doc["msg"] = "ALERT FROM HAB";
        #else
        char msg[32];
        sprintf(msg, "ALERT FROM %s", chipID);
        doc["msg"] = msg;
        #endif
        doc["active"] = state;

        char serialisedJSON[JSON_SERIALISATION_LIMIT];
        serializeJson(doc, serialisedJSON, JSON_SERIALISATION_LIMIT);
        sendMessage("ALERT", serialisedJSON);

        #if defined(HAB_SYSTEM)
        forwardPacket("ALERT", serialisedJSON);
        #endif

        Serial.println(serialisedJSON);

        #if defined(REMOTE_SYSTEM)

        if(state) {
            warningLED(true, false, true);
        } else {
            warningLED(false, false, false);
        }
        #endif

        lastAlertMS = millis();
    }
}

#endif