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

PubSubClient client(MQTT_HOST_SERVER, MQTT_HOST_PORT, mqttCallback, wifiClient);

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

void forwardPacket(const char *topic, const char *payload) {
    Serial.print("Forwarding message to ");
    Serial.print(topic);
    Serial.print(" ");
    Serial.println(payload);
    client.publish(topic, payload);
}

void processPingPacket(const char *payload, float rssi) {
    Serial.println("Processing ping packet");
    Serial.print("RSSI: ");
    Serial.println(rssi);
    // Inplant RSSI into json payload
    StaticJsonDocument<JSON_SERIALISATION_LIMIT> doc;
    deserializeJson(doc, payload);
    doc["rssi"] = rssi;
    char outgoing[JSON_SERIALISATION_LIMIT];
    serializeJson(doc, outgoing);
    forwardPacket("PING", outgoing);
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");

		// Check if Wifi is connected
		if ((WiFi.status() != WL_CONNECTED)) {
			WiFi.disconnect();
			WiFi.reconnect();
			delay(500);
			return reconnect();
		}

		// Attempt to connect
		String clientId = "ESP8266Client-";
		clientId += String(random(0xffff), HEX);
		
		if (client.connect(clientId.c_str())) {
			Serial.println("MQTT Connected");
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
    }
}

void loopHab() {
    if (!client.connected()) {
	reconnect();
    }
    client.loop();
}

#endif