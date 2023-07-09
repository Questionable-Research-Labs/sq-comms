#include "heltec.h"
#include "config.h"
#include "lora.h"
#include "display.h"
#include "alert.h"

#if defined(HAB_SYSTEM)
#include "hab.h"
#endif

void setup() {
    // WIFI Kit series V1 not support Vext control
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, LORA_BAND /*long BAND*/);

	setupLora();
    Serial.println("LoRa init succeeded.");
	initDisplay();
	Serial.println("Display init succeeded.");

	#if defined(HAB_SYSTEM)
	setupHab();
	Serial.println("HAB init succeeded.");
	#else
	Serial.println("HAB init skipped.");
	#endif
}


byte msgCount= 0;	// count of outgoing messages
int interval = 2000;	// interval between sends
long lastSendTime = 0;	// time of last packet send

void loop() {
	#if !defined(HAB_SYSTEM)

    if (millis() - lastSendTime > interval) {
		sendAlert()
		lastSendTime = millis();  //void loopHab() timestamp the message
		interval = 1000;	  // 1 second
		msgCount++;
    }
	#endif

    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
	#if defined(HAB_SYSTEM)
		loopHab();
	#else
		sensorLoop();
	#endif

}