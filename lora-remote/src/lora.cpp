#include <lora.h>

const size_t handled_packet_max_size = 32;
const size_t handled_packet_hash_size = 32;
uint32_t handled_packet[handled_packet_max_size] = {0};
size_t handled_packet_head = 0;	 // size_t is an unsigned long

#define LoRa_MOSI 27
#define LoRa_MISO 19
#define LoRa_SCK 5

#define CHANNEL_BUSY 1
#define UNKOWN_ERROR 20

SX1276 radio = new Module(18, 26, 9, RADIOLIB_NC);

#define MAX_OUT_QUEUE_LENGTH 16
char outgoingQueueMessages[MAX_OUT_QUEUE_LENGTH][JSON_SERIALISATION_LIMIT] = {};
int outgoingQueueHead = 0;
int outgoingQueueTail = 0;
bool outgoingQueueFull = false;


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
	if (!_enableInterrupt) {
		return;
	}
	newLoraMessageWaiting = true;
}

void addMessageToOutQueue(const char* outgoing) {
	// Add to Circular queue with head and tail
	// If full, overwrite oldest message

	size_t outgoingLength = strlen(outgoing);
    if (outgoingLength > JSON_SERIALISATION_LIMIT) {
        Serial.println("Tried to send oversize packet");
    }

	// If queue is full, overwrite oldest message
	if (outgoingQueueFull) {
		// Increment tail
		outgoingQueueTail = (outgoingQueueTail + 1) % MAX_OUT_QUEUE_LENGTH;
		Serial.println("Overwriting oldest packet");
	}

	// Copy message to queue
	memcpy(outgoingQueueMessages[outgoingQueueHead], outgoing, outgoingLength+1);

	// Increment head
	outgoingQueueHead = (outgoingQueueHead + 1) % MAX_OUT_QUEUE_LENGTH;

	// Check if queue is full
	if (outgoingQueueHead == outgoingQueueTail) {
		outgoingQueueFull = true;
	}
}

void proccessOutQueueCycle() {
	// Get oldest packet in queue and send
	// If queue is empty, do nothing

	if (outgoingQueueHead == outgoingQueueTail && !outgoingQueueFull) {
		// Queue is empty
		return;
	}

	// Get oldest packet
	char* outgoing = outgoingQueueMessages[outgoingQueueTail];

	// Send packet
	int status = sendMessage(outgoing);
	if (status == CHANNEL_BUSY) {
		Serial.println("Not removing packet from queue as Serial was busy");
	} else if (status == UNKOWN_ERROR) {
		Serial.println("Not removing packet from queue as unknown error");
	} else {
		// Remove packet from queue
		outgoingQueueTail = (outgoingQueueTail + 1) % MAX_OUT_QUEUE_LENGTH;
		outgoingQueueFull = false;
	}
}

int sendMessage(const char* outgoing) {
    Serial.println("Sending message: ");
    Serial.println(outgoing);

	if (newLoraMessageWaiting || !_enableInterrupt) {
		Serial.println("New message waiting, not sending.");
		return CHANNEL_BUSY;
	}

	float live_rssi = radio.getRSSI(false);
	if (live_rssi > -70) {
		// Already someone transmitting, start task in a random amount of time
		Serial.println("[CSMA] Channel busy, waiting ...");
		Serial.printf("status: %f/-70, newLoraMessageWaiting: %d, _enableInterrupt: %d\n", live_rssi, newLoraMessageWaiting, _enableInterrupt);

		return CHANNEL_BUSY;
	} else {
		Serial.println("Transmitting Message ...");
		radio.standby();
		delay(50);
		_enableInterrupt = false;

		unsigned long startTime = millis();
		// No one transmitting, send message
		radio.transmit(outgoing);
		unsigned long endTime = millis();
		delay(50);
		_enableInterrupt = true;

		radio.startReceive();

		Serial.print("Message sent in ");
		Serial.print(endTime - startTime);
		Serial.println("ms");

		return 0;
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
    addMessageToOutQueue(outgoing);
}
void sendMessage(const char* topic, const char* payload) {
    sendMessage(topic, "[]", payload, 0);
}

void LoRaCheckForPacket() {
    // Serial.printf("RSSI: %f | Snr: %f\n", radio.getRSSI(false), radio.getSNR());
	// Debug message with queue status
	// Serial.printf("Queue: Head: %d, Tail: %d, Full: %d\n", outgoingQueueHead, outgoingQueueTail, outgoingQueueFull);

	if (!newLoraMessageWaiting) {
		proccessOutQueueCycle();
		return;
	}
	newLoraMessageWaiting = false;

	_enableInterrupt = false;
	
	#ifdef OUTPUT_NEOPIXEL
	flashNeoPixels();
	#endif


    // read packet header bytes:
    String incoming = "";
    int state = radio.readData(incoming);


	_enableInterrupt = true;
	radio.startReceive();

    Serial.println("\n######## LORA INCOMING PACKET ########");
    Serial.printf("Message: %s\nRSSI: %f | Snr: %f\n", incoming.c_str(), radio.getRSSI(), radio.getSNR());
	displayUpdateLoraStats(radio.getRSSI());

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
    int maxHopCount = 0;
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
    } else {
		if (strcmp(topic, "ALERT") == 0) {
			processAlert(hopsData, data, packetIDInt, radio.getRSSI());
		}
		forwardPacket(topic, data);
    }
#endif

#if defined(REMOTE_SYSTEM)
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