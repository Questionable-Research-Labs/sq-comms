#include <lora.h>

void setupLora() {
    LoRa.setSyncWord(0xe4);  // ranges from 0-0xFF, default 0x34, see API docs
    LoRa.setCodingRate4(8);
    LoRa.enableCrc();
}

void sendRawMessage(char* outgoing) {
    LoRa.beginPacket();	   // start packet
    LoRa.print(outgoing);  // add payload
    LoRa.endPacket();	   // finish packet and send it
}

void sendMessage(const char* topic, const char* payload) {
    char outgoing[JSON_SERIALISATION_LIMIT];
    sprintf(outgoing, "%s:%s", topic, payload);
    sendRawMessage(outgoing);
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


    const char* loraMessage = incoming.c_str();

    const char *ptr = strchr(loraMessage, ':');
    if (!ptr) {
        Serial.println("No topic found in message");
        return;
    }
    int messageSplitIndex = ptr - loraMessage;

    Serial.println("Topic split: " + String(messageSplitIndex));
    
    // Get the topic and data out of the incoming packet
    char topic[messageSplitIndex + 2] = "";
    memcpy(topic, &incoming[0], messageSplitIndex);
    topic[messageSplitIndex + 1] = '\0';
    Serial.println(topic);

    // Rest of the message is incoming
    char data[1024] = "";
    memcpy(data, &incoming[messageSplitIndex + 1], strlen(loraMessage) - messageSplitIndex);
    Serial.println(data);

    for (int i = 0; i<20; i++) {
        Serial.print(incoming[i], HEX);
    }

    #if defined(HAB_SYSTEM)
        forwardPacket(topic, data);
    #endif
}
