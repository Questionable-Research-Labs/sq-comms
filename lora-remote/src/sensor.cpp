#include <sensor.h>
#ifdef SENSOR_SYSTEM

#define DHTTYPE DHT11
uint32_t delayMS;

SoftwareSerial mainBoard(12, 21); // RX, TX

void sensorSetup() {
    Serial.println("Setting up sensors (serial lmao)");

    mainBoard.begin(9600);  
}

void sensorLoop() {

    // check if main board has sent data, it should come in a JSON format like this: {"temp": 20.0, "hum": 50.0, "dust": 100.0}
    if (mainBoard.available()) {
        // read the data
        String data = mainBoard.readStringUntil('\n');

        // print
        Serial.println("RECIEVED: " + data + "\n");

        // parse the data
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, data);

        // add the data to the JSON document send {"from":"70CD55A4AE30","readings":[{"sensorName":"temperature","value":23.10000038}, {"sensorName":"humidity","value":50.5}, {"sensorName":"dust","value":100.0}]}
        // create new doc with the data

        DynamicJsonDocument doc2(1024);

        doc2["from"] = chipID;

        doc2["readings"][0]["sensorName"] = "temperature";
        doc2["readings"][0]["value"] = doc["temp"];
        doc2["readings"][1]["sensorName"] = "humidity";
        doc2["readings"][1]["value"] = doc["hum"];
        doc2["readings"][2]["sensorName"] = "dust";
        doc2["readings"][2]["value"] = doc["dust"];


        // serialise the JSON document
        char serialisedJSON[1024];
        serializeJson(doc2, serialisedJSON, 1024);

        // print the serialised JSON document
        Serial.println("SENDING: " + String(serialisedJSON));

        // send the JSON document to the main board
        sendMessage("SENSOR", serialisedJSON);
    }
}

#endif