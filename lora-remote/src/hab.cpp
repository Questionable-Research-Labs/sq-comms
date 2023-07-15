#include <hab.h>
#if defined(HAB_SYSTEM)

#define WIFI_CONFIG_LENGTH 40

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
PubSubClient client(wifiClient);

char mqtt_server[WIFI_CONFIG_LENGTH] = DEFAULT_MQTT_HOST_SERVER;
char mqtt_port[WIFI_CONFIG_LENGTH] = DEFAULT_MQTT_HOST_PORT;
char mqtt_user[WIFI_CONFIG_LENGTH] = "";
char mqtt_pass[WIFI_CONFIG_LENGTH] = "";

bool shouldSaveConfig = false;

void saveConfigCallback () {
  Serial.println("Saving MQTT config");
  shouldSaveConfig = true;
}

int loadConfig() {
    Serial.println("Loading config");
    if (SPIFFS.begin(true)) {
        Serial.println(" -> Mounted file system");
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
            Serial.println(" -> Reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile) {
                Serial.println(" -> Opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);
                configFile.readBytes(buf.get(), size);
                DynamicJsonDocument doc(1024);
                auto error = deserializeJson(doc, buf.get());
                if (error) {
                    Serial.println(" -> Failed to load json config");
                } else {
                    Serial.println(" -> Successfully parsed config");
                    strcpy(mqtt_server, doc["mqtt_server"]);
                    strcpy(mqtt_port, doc["mqtt_port"]);
                    strcpy(mqtt_user, doc["mqtt_user"]);
                    strcpy(mqtt_pass, doc["mqtt_pass"]);

                    Serial.print(" -> MQTT URI: ");
                    Serial.printf("%s:%s\n", mqtt_server, mqtt_port);
                    Serial.print(" -> MQTT cred: ");
                    Serial.printf("%s:%s\n", mqtt_user, mqtt_pass);
                    
                }
                configFile.close();
            }
        }
    } else {
        Serial.println("failed to mount FS");
        return 1;
    }
    return 0;
}

void saveConfig() {
    Serial.println("Saving config");
    DynamicJsonDocument doc(1024);
    doc["mqtt_server"] = mqtt_server;
    doc["mqtt_port"] = mqtt_port;
    doc["mqtt_user"] = mqtt_user;
    doc["mqtt_pass"] = mqtt_pass;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
    }
    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("Saved config");
}

void setupHab() {
    Serial.println("HAB System");
    if (loadConfig()) {
        Serial.println("Failed to load config");
        delay(3000);
        ESP.restart();
    }

                        Serial.print(" -> MQTT URI: ");
                    Serial.printf("%s:%s\n", mqtt_server, mqtt_port);
                    Serial.print(" -> MQTT cred: ");
                    Serial.printf("%s:%s\n", mqtt_user, mqtt_pass);


    Serial.println("Initialising WifiManager");
    // WiFiManager
    // Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    // reset saved settings
    // wifiManager.resetSettings();
    wifiManager.setConnectRetries(6);
    wifiManager.setDarkMode(true);
    wifiManager.setCountry("NZ");
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    WiFiManagerParameter custom_mqtt_server("mqtt_server", "MQTT Host", mqtt_server, WIFI_CONFIG_LENGTH);
    WiFiManagerParameter custom_mqtt_port("mqtt_port", "MQTT Port", DEFAULT_MQTT_HOST_PORT, WIFI_CONFIG_LENGTH);
    WiFiManagerParameter custom_mqtt_user("mqtt_user", "MQTT User", mqtt_user, WIFI_CONFIG_LENGTH);
    WiFiManagerParameter custom_mqtt_pass("mqtt_pass", "MQTT Password", mqtt_pass, WIFI_CONFIG_LENGTH);

    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_user);
    wifiManager.addParameter(&custom_mqtt_pass);

    if (!wifiManager.autoConnect()) {
        Serial.println("Failed to connect and hit timeout");
        delay(3000);
        ESP.restart();
        delay(5000);
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    strcpy(mqtt_user, custom_mqtt_user.getValue());
    strcpy(mqtt_pass, custom_mqtt_pass.getValue());
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    strcpy(mqtt_port, custom_mqtt_port.getValue());
    client.setServer(mqtt_server, atoi(mqtt_port));
    client.setCallback(mqttCallback);

    if (shouldSaveConfig) {
        saveConfig();
    }

    reconnect();
}

void forwardPacket(const char *topic, const char *payload) {
    Serial.print("Forwarding message to ");
    Serial.print(topic);
    Serial.print(" ");
    Serial.println(payload);
    client.publish(topic, payload);
}

void processPingPacket(const char *payload, float rssi) {
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
		Serial.println(clientId);
        Serial.println(mqtt_user);
        Serial.println(mqtt_pass);

		if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
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