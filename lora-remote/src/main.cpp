#include "heltec.h"
#define BAND 433E6  // you can set band here directly,e.g. 868E6,915E6

byte msgCount = 0;	// count of outgoing messages
int interval = 2000;	// interval between sends
long lastSendTime = 0;	// time of last packet send

void sendMessage(String outgoing) {
    LoRa.beginPacket();	   // start packet
    LoRa.print(outgoing);  // add payload
    LoRa.endPacket();	   // finish packet and send it
    msgCount++;		   // increment message ID
}

void onReceive(int packetSize) {
    if (packetSize == 0) return;  // if there's no packet, return

    // read packet header bytes:
    String incoming = "";

    while (LoRa.available()) {
	incoming += (char)LoRa.read();
    }

    Serial.println("Message: " + incoming);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
    Serial.println();
    Heltec.display->clear();
    Heltec.display->drawString(0, 10, incoming);
    Heltec.display->display();
}

void setup() {
    // WIFI Kit series V1 not support Vext control
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

    LoRa.setSyncWord(0xe4);  // ranges from 0-0xFF, default 0x34, see API docs
    Serial.println("LoRa init succeeded.");
    Heltec.display->init();
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->clear();

    Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
    Heltec.display->drawString(0, 10, "Wait for incoming data...");
    Heltec.display->display();
}

void loop() {
    if (millis() - lastSendTime > interval) {
	String message = "Alert #";  // send a message
	message += msgCount;
	sendMessage(message);
	Serial.println("Sending " + message);
	lastSendTime = millis();  // timestamp the message
	interval = 1000;	  // 1 second
	msgCount++;
    }

    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
}

String rssi = "RSSI --";
String packSize = "--";
String packet;

void LoRaData() {
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 15, "Received " + packSize + " bytes");
    Heltec.display->drawStringMaxWidth(0, 26, 128, packet);
    Heltec.display->drawString(0, 0, rssi);
    Heltec.display->display();
}