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

#if defined(OUTPUT_NEOPIXEL)
#include "neolights.h"
#endif

void setup() {
    // WIFI Kit series V1 not support Vext control

	Serial.begin(115200);

	setChipId();
	Serial.print("Chip ID booting: ");
	Serial.println(chipID);

	initDisplay();

	#if defined(OUTPUT_NEOPIXEL)
	setupNeoPixels();
	Serial.println("Lights init succeeded.");
	#else
	Serial.println("Lights init skipped.");
	#endif

	setupLora();
    Serial.println("LoRa init succeeded.");

	#if defined(HAB_SYSTEM)
	setupHab();
	Serial.println("HAB init succeeded.");
	#else
	Serial.println("HAB init skipped.");
	#endif

	#if defined(SENSOR_SYSTEM)
		sensorSetup();
	#endif

	#if defined(REMOTE_SYSTEM)
		alertSetup();
		setupLED();
	#endif


	Serial.println("\n########## Setup complete ##########\n");
}


byte msgCount= 0;	// count of outgoing messages
int interval = 5000;	// interval between sends
long lastSendTime = 0;	// time of last packet send

void loop() {
	#if defined(ALERT_SYSTEM)
	sendAlert();
	#endif

	#if !defined(HAB_SYSTEM)

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

	#if defined(OUTPUT_NEOPIXEL)
		animateNeoPixel();
	#endif

	renderState();
}