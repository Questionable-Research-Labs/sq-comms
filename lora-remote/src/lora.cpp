#include <lora.h>

const size_t handled_packet_max_size = 32;
const size_t handled_packet_hash_size = 32;
uint32_t handled_packet[handled_packet_max_size] = {0};
size_t handled_packet_head = 0;	 // size_t is an unsigned long

#define LoRa_MOSI 27
#define LoRa_MISO 19
#define LoRa_SCK 5

SX1276 radio = new Module(18, 26, 9, RADIOLIB_NC);

void setupLora() {
    Serial.print(F("[LoRa] Initializing ... "));
    Serial.print(F(" -> [SPI] Initializing ... "));
    SPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI);
    Serial.print(F(" -> [SX1276] Initializing ... "));

    int state = radio.begin();

    if (state == RADIOLIB_ERR_NONE) {
		Serial.println(F("success!"));
    } else {
		Serial.print(F("failed, code "));
		Serial.println(state);
		while (true);
    }

	radio.setSyncWord(0xe4);
	radio.setCodingRate(7);
	radio.setSpreadingFactor(7);
	radio.setCRC(true);
	

	radio.setPacketReceivedAction(setNewMessageFlag);

	state = radio.startReceive();
	if (state == RADIOLIB_ERR_NONE) {
		Serial.println(F("success!"));
	} else {
		Serial.print(F("failed, code "));
		Serial.println(state);
		while (true);
	}
}

bool _enableInterrupt = true;
bool newLoraMessageWaiting = false;

ICACHE_RAM_ATTR
void setNewMessageFlag() {
	if (newLoraMessageWaiting) {
		Serial.println("New message waiting, but flag already set.");
	}
	if (!_enableInterrupt) {
		Serial.println("New message waiting, but interrupt disabled.");
		return;
	}
	newLoraMessageWaiting = true;
}

void sendMessage(const char* outgoing) {
    Serial.println("Sending message: ");
    Serial.println(outgoing);

	int16_t status = radio.scanChannel();
	if (status == RADIOLIB_PREAMBLE_DETECTED) {
		// Already someone transmitting, start task in a random amount of time
		Serial.println("[CSMA] Channel busy, waiting ...");
		delay(random(10,100));

		// Retry
		sendMessage(outgoing);
	} else if (status == RADIOLIB_CHANNEL_FREE) {
		Serial.println("Transmitting Message ...");
		unsigned long startTime = millis();
		// No one transmitting, send message
		radio.transmit(outgoing);
		unsigned long endTime = millis();
		Serial.print("Message sent in ");
		Serial.print(endTime - startTime);
		Serial.println("ms");
	} else {
		// Unkown Error
		Serial.print("Unknown error when sending message, code: ");
		Serial.println(status);
	}
}

void sendMessage(const char* topic, const char* hops, const char* payload, uint32_t packetID) {
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
void sendMessage(const char* topic, const char* payload) {
    sendMessage(topic, "[]", payload, 0);
}

void LoRaCheckForPacket() {
	if (!newLoraMessageWaiting) {
		return;
	}
	newLoraMessageWaiting = false;

	_enableInterrupt = false;

    // read packet header bytes:
    String incoming = "";
    int state = radio.readData(incoming);

	_enableInterrupt = true;
	radio.startReceive();

    Serial.println("\n######## LORA INCOMING PACKET ########");
    Serial.printf("Message: %s\nRSSI: %f | Snr: %f\n", incoming.c_str(), radio.getRSSI(), radio.getSNR());

    if (state == RADIOLIB_ERR_CRC_MISMATCH) {
		Serial.println(F("[SX1276] CRC error!"));
		return;
	} else if (state != RADIOLIB_ERR_NONE) {
      // some other error occurred
      Serial.print(F("[SX1276] Failed, code "));
      Serial.println(state);
	  return;
    }

    // Heltec.display->clear();
    // Heltec.display->drawString(0, 10, incoming);
    // Heltec.display->display();

    parsePacket(incoming.c_str());
}

void sendPing() {
    DynamicJsonDocument doc(1024);
    doc["from"] = chipID;
    doc["uptime"] = millis();

    char serialisedJSON[1024];
    serializeJson(doc, serialisedJSON, 1024);
    Serial.println(serialisedJSON);
    sendMessage("PING", serialisedJSON);
}

static topicPriority msgTypeMap[] = {
    {"SENSOR", 2},
    {"ALERT", -1},
    {"PING", 3},
    {"UAB", 2},
    {"ECHO", 0}};

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
    // Scan for break points
    int messageSplitIndex[32];
    int messageSplitsCount = scanForBreak(messageSplitIndex, loraMessage, strlen(loraMessage), "[;]", 32);
    if (messageSplitsCount < 3) {
	Serial.println("Invalid message format!");
	return;
    }

    // Get the packet ID out of the incoming packet
    char* packetID = copyCharRange(loraMessage, 0, messageSplitIndex[0]);
    uint32_t packetIDInt = strtoul(packetID, NULL, 10);

    // Get the topic and data out of the incoming packet
    char* topic = copyCharRange(loraMessage, messageSplitIndex[0] + 3, messageSplitIndex[1]);

    // Is the packet already in the handled pile?
    for (int i = 0; i < handled_packet_max_size; i++) {
	if (handled_packet[i] == packetIDInt) {
	    Serial.println("Packet already handled!");
#if defined(HAB_SYSTEM)
	    // Need to get multiple paths in order to geolocate, so we want duplicate transmissions
	    // Check if topic is ALERT
	    if (strcmp(topic, "ALERT") == 0) {
		// If it is, we want to locate it
		Serial.println("Keeping ALERT packet, Locating...");
	    } else {
		return;
	    }
#else
	    return;
#endif
	}
    }

    // Add it to the handled pile
    handled_packet[handled_packet_head] = packetIDInt;
    handled_packet_head++;
    if (handled_packet_head >= handled_packet_max_size) {
	handled_packet_head = 0;
    }

    // Get the hops data out of the incoming packet
    char* hopsData = copyCharRange(loraMessage, messageSplitIndex[1] + 3, messageSplitIndex[2]);

    // Rest of the message is incoming
    char* data = copyCharRange(loraMessage, messageSplitIndex[2] + 3, strlen(loraMessage));

#if defined(HAB_SYSTEM)
    if (strcmp(topic, "PING") == 0) {
	processPingPacket(data, radio.getRSSI());
    } else if (strcmp(topic, "ALERT") == 0) {
	processAlert(hopsData, data, packetIDInt, radio.getRSSI());
    } else {
	forwardPacket(topic, data);
    }
#endif

#if defined(ALERT_SYSTEM)
    forwardLED(topic, data);
#endif

// Forward packet if needed
#if !defined(HAB_SYSTEM)
    relayPacket(topic, hopsData, data, packetIDInt);
#endif

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
    Serial.printf("Hops count: %d/%d\n", hopsCount, maxHopCount);

    // Add this chip to the hops data
    doc.createNestedObject();
    doc[hopsCount]["from"] = chipID;
    doc[hopsCount]["rssi"] = radio.getRSSI();
    doc[hopsCount]["order"] = hopsCount;

    // Serialise and relay packet
    char serialisedJSON[512];
    serializeJson(doc, serialisedJSON, 512);
    sendMessage(topic, serialisedJSON, data, packetID);
}