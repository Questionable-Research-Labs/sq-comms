#include <sensor.h>
#ifdef SENSOR_SYSTEM

void sensorSetup() {
    Serial.println("Setting up sensors");
}

void sensorLoop() {
    DynamicJsonDocument doc(1024);

    doc["from"] = "ChipAddrHere";
    doc["readings"][0]["sensorName"] = "temperature";
    doc["readings"][0]["value"] = random(0, 100);


    char serialisedJSON[1024];
    serializeJson(doc, serialisedJSON, 1024);
    sendMessage("SENSOR", serialisedJSON);
}

#endif