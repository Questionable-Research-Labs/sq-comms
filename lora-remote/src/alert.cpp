#include <alert.h>

#define SWITCH_PIN 36
bool prevState = false;
bool state = false;

void alertSetup() {
    Serial.println("Setting up alert system");
    pinMode(SWITCH_PIN, INPUT);
}

void sendAlert() {
    DynamicJsonDocument doc(1024);

    // query switch
    // if switch is on or toggled, send alert message, either true or false

    bool state = digitalRead(SWITCH_PIN);
    if (state != prevState) {

        doc["from"] = chipID;
        doc["msg"] = "test alert";
        doc["active"] = state;

        char serialisedJSON[1024];
        serializeJson(doc, serialisedJSON, 1024);
        sendMessage("ALERT", serialisedJSON);

        Serial.println(serialisedJSON);

        prevState = state;
        Serial.println("Switch toggled to " + String(state));

        if(state) {
            warningLED(true, false, true);
        } else {
            warningLED(false, false, false);
        }
    }    
}
