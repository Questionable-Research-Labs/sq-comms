#include <hab.h>
#if defined(HAB_SYSTEM)


void mqttCallback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (unsigned int i = 0; i < length; i++) {
	Serial.print((char)payload[i]);
	}
	Serial.println();
}

WiFiClient wifiClient;

IPAddress mqttServer = IPAddress(MQTT_HOST_SERVER);
PubSubClient client(mqttServer, MQTT_HOST_PORT, mqttCallback, wifiClient);

void setupHab() {
	// WiFiManager
	// Local intialization. Once its business is done, there is no need to keep it around
	WiFiManager wifiManager;

	// reset saved settings
	// wifiManager.resetSettings();
	wifiManager.setConnectRetries(6);
	wifiManager.setDarkMode(true);
	wifiManager.setCountry("NZ");
	wifiManager.autoConnect();

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("no");
}

#endif