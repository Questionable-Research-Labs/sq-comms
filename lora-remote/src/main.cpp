#include "heltec.h"
#include "config.h"
#include "lora.h"
#include "display.h"

byte msgCount = 0;	// count of outgoing messages
int interval = 2000;	// interval between sends
long lastSendTime = 0;	// time of last packet send


void setup() {
    // WIFI Kit series V1 not support Vext control
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, LORA_BAND /*long BAND*/);

	setupLora();
    Serial.println("LoRa init succeeded.");
	initDisplay();
	Serial.println("Display init succeeded.");
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