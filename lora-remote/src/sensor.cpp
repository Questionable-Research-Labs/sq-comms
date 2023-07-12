#include <sensor.h>
#ifdef SENSOR_SYSTEM

#define DHTTYPE DHT22
uint32_t delayMS;

#define DHTPIN 13

DHT_Unified dht(DHTPIN, DHTTYPE);

void sensorSetup() {
    //Serial.println("Setting up sensors");
    dht.begin();
    
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
}

void sensorLoop() {
    DynamicJsonDocument doc(1024);

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        //Serial.println(F("Error reading temperature!"));
    }
    else {
        //Serial.print(F("Temperature: "));
        //Serial.print(event.temperature);
        //Serial.println(F("°C"));
    }

    doc["from"] = chipID;
    doc["readings"][0]["sensorName"] = "temperature";
    doc["readings"][0]["value"] = event.temperature;

    char serialisedJSON[1024];
    serializeJson(doc, serialisedJSON, 1024);
    sendMessage("SENSOR", serialisedJSON);
}

#endif