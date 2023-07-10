#include <lora.h>

typedef struct {
    char* topic;
    int hopCount;
} topicPriority;

const size_t handled_packet_max_size = 32;
const size_t handled_packet_hash_size = 32;
uint32_t handled_packet[handled_packet_max_size] = {0};
size_t handled_packet_head = 0;             // size_t is an unsigned long

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

void sendMessage(char* topic, char* hops, char* payload, uint32_t packetID) {
    char outgoing[JSON_SERIALISATION_LIMIT];

    while (packetID == 0) {
        packetID = esp_random();
    }
    handled_packet[handled_packet_head] = packetID;
    handled_packet_head++;
    if (handled_packet_head >= handled_packet_max_size) {
        handled_packet_head = 0;
    }

    sprintf(outgoing, "%u[;]%s[;]%s[;]%s", packetID, topic, hops, payload);
    sendMessage(outgoing);
}
void sendMessage(char* topic, char* payload) {
    sendMessage(topic, "[]", payload, 0);
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
    {"UAB", 2},
    {"ECHO", 0}
};

int getMaxHopCount(char* topic) {
    int maxHopCount = -1;
    for (int i = 0; i < sizeof(msgTypeMap) / sizeof(topicPriority); i++) {
        if (strcmp(topic, msgTypeMap[i].topic) == 0) {
            maxHopCount = msgTypeMap[i].hopCount;
            break;
        }
    }
    return maxHopCount;
}

void parsePacket(const char* loraMessage) {
    Serial.println("Parsing packet...");
    // Scan for break points
    int messageSplitIndex[32];
    int messageSplitsCount = scanForBreak(messageSplitIndex, loraMessage, strlen(loraMessage), "[;]", 32);
    if (messageSplitsCount < 3) {
        Serial.println("Invalid message format!");
        return;
    }

    Serial.println("PacketID split: " + String(messageSplitIndex[0]));

    // Get the packet ID out of the incoming packet
    char* packetID = copyCharRange(loraMessage, 0, messageSplitIndex[0]);
    uint32_t packetIDInt = strtoul(packetID, NULL, 10);

    // Is the packet already in the handled pile?
    for (int i = 0; i < handled_packet_max_size; i++) {
        if (handled_packet[i] == packetIDInt) {
            Serial.println("Packet already handled!");
            return;
        }
    }

    // Add it to the handled pile
    handled_packet[handled_packet_head] = packetIDInt;
    handled_packet_head++;
    if (handled_packet_head >= handled_packet_max_size) {
        handled_packet_head = 0;
    }

    // Get the topic and data out of the incoming packet
    char* topic = copyCharRange(loraMessage, messageSplitIndex[0]+3, messageSplitIndex[1]);

    // Get the hops data out of the incoming packet
    char* hopsData = copyCharRange(loraMessage, messageSplitIndex[1]+3, messageSplitIndex[2]);

    // Rest of the message is incoming
    char* data = copyCharRange(loraMessage, messageSplitIndex[2]+3, strlen(loraMessage));


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

    // Forward packet if needed
    relayPacket(topic, hopsData, data, packetIDInt);

    // Cleanup
    delete[] topic;
    delete[] data;
    delete[] hopsData;
}

void relayPacket(char* topic, char* hopsData, char* data, uint32_t packetID) {
    // Parse the json hops data
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, hopsData);
    if (error) {
        Serial.println("Failed to parse hops data");
        return;
    }
    // Is in the following format:
    // [{from: "chipID", rssi: "56.2", order: "0"}, {from: "chipID2", rssi: "5.2", order: "1"}]
    int hopsCount = doc.size();
    int maxHopCount = getMaxHopCount(topic);
    if (maxHopCount == -1) {
        Serial.println("Invalid topic!");
        return;
    }
    if ((hopsCount + 1 >= maxHopCount) && maxHopCount != -1) {
        Serial.println("Max hop count reached! No need to repeat");
        return;
    }
    Serial.printf("Hops count: %d/%d\n", hopsCount,maxHopCount);

    // Add this chip to the hops data
    doc.createNestedObject();
    doc[hopsCount]["from"] = chipID;
    doc[hopsCount]["rssi"] = LoRa.packetRssi();
    doc[hopsCount]["order"] = hopsCount;

    // Serialise and relay packet
    char serialisedJSON[512];
    serializeJson(doc, serialisedJSON, 512);
    sendMessage(topic, serialisedJSON, data, packetID);
}