#include <lora.h>

typedef struct {
    char* topic;
    int hopCount;
} topicPriority;

void setupLora() {
    LoRa.setSyncWord(0xe4);  // ranges from 0-0xFF, default 0x34, see API docs
    LoRa.setCodingRate4(8);
    LoRa.enableCrc();
}

void sendMessage(char* outgoing) {
    Serial.println("Sending message: ");
    Serial.println(outgoing);

    LoRa.beginPacket();	   // start packet
    LoRa.print(outgoing);  // add payload
    LoRa.endPacket();	   // finish packet and send it
}

void sendMessage(char* topic, char* hops, char* payload) {
    char outgoing[JSON_SERIALISATION_LIMIT];
    sprintf(outgoing, "%s[;]%s[;]%s", topic, hops, payload);
    sendMessage(outgoing);
}
void sendMessage(char* topic, char* payload) {
    sendMessage(topic, "[]", payload);
}

void onReceive(int packetSize) {
    if (packetSize == 0) return;  // if there's no packet, return

    // read packet header bytes:
    String incoming = "";

    while (LoRa.available()) {
	incoming += (char)LoRa.read();
    }

    Serial.println("Message: " + String(incoming));
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
    Serial.println();
    Heltec.display->clear();
    Heltec.display->drawString(0, 10, incoming);
    Heltec.display->display();

    parsePacket(incoming.c_str());
}

void sendPing() {
    DynamicJsonDocument doc(1024);
    doc["from"] = chipID;
    doc["uptime"] = millis();

    char serialisedJSON[1024];
    serializeJson(doc, serialisedJSON, 1024);
    Serial.println("Sending Ping");
    Serial.println(serialisedJSON);
    sendMessage("PING", serialisedJSON);
}

static topicPriority msgTypeMap[] = {
    {"SENSOR", 2},
    {"ALERT", -1},
    {"PING", 3},
};

// Have to call delete[] on the result of this function
char* copyCharRange(const char* src, int start, int end) {
    int length = end - start + 1;
    char* result = new char[length];
    memcpy(result, &src[start], length);
    result[length - 1] = '\0';
    return result;
}

// Scan for sub string '[;]' in a char array, and return all results
int scanForBreak(int* results, const char* src, int srcLength, const char* breakString, int maxLength) {
    int breakLength = strlen(breakString);
    int resultIndex = 0;
    for (int i = 0; i < srcLength; i++) {
        if (strncmp(&src[i], breakString, breakLength) == 0) {
            results[resultIndex] = i;
            resultIndex++;
        }
        if (resultIndex >= maxLength) {
            return resultIndex;
        }
    }
    return resultIndex;
}

void parsePacket(const char* loraMessage) {
    Serial.println("Parsing packet...");
    // Scan for break points
    int messageSplitIndex[32];
    int messageSplitsCount = scanForBreak(messageSplitIndex, loraMessage, strlen(loraMessage), "[;]", 32);
    if (messageSplitsCount < 2) {
        Serial.println("Invalid message format!");
        return;
    }


    Serial.println("Topic split: " + String(messageSplitIndex[0]));

    // Get the topic and data out of the incoming packet
    char* topic = copyCharRange(loraMessage, 0, messageSplitIndex[0]);

    // Get the hops data out of the incoming packet
    char* hopsData = copyCharRange(loraMessage, messageSplitIndex[0]+3, messageSplitIndex[1]);

    // Rest of the message is incoming
    char* data = copyCharRange(loraMessage, messageSplitIndex[1]+3, strlen(loraMessage));

    Serial.println(topic);
    Serial.println(hopsData);
    Serial.println(data);


    

#if defined(HAB_SYSTEM)
    if (strcmp(topic, "PING") == 0) {
	processPingPacket(data, LoRa.packetRssi());
    } else {
	forwardPacket(topic, data);
    }
#endif

#if defined(ALERT_SYSTEM)
    forwardLED(topic, data);
#endif

    delete[] topic;
    delete[] data;

}