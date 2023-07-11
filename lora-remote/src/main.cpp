#include "config.h"
#include "lora.h"
#include "display.h"
#include "alert.h"
#include "sensor.h"
#include "chipID.h"
#include "warningled.h"

#if defined(HAB_SYSTEM)
#include "hab.h"
#endif

void setup() {
    // WIFI Kit series V1 not support Vext control

	Serial.begin(115200);

	setChipId();
	Serial.print("Chip ID booting: ");
	Serial.println(chipID);

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

	#if defined(SENSOR_SYSTEM)
		sensorSetup();
	#endif

	#if defined(ALERT_SYSTEM)
		alertSetup();
		setupLED();
	#endif
}


byte msgCount= 0;	// count of outgoing messages
int interval = 2000;	// interval between sends
long lastSendTime = 0;	// time of last packet send

void loop() {
	#if !defined(HAB_SYSTEM)
	// sendAlert();

    if (millis() - lastSendTime > interval) {
		sendPing();
		lastSendTime = millis();  //void loopHab() timestamp the message
		msgCount++;
    }
	#endif

    // parse for a packet, and call onReceive with the result:
    LoRaCheckForPacket();

	#if defined(HAB_SYSTEM)
		loopHab();
	#elif defined(SENSOR_SYSTEM)
		sensorLoop();
	#endif

}