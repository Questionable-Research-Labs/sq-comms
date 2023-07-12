#pragma once

#define LORA_BAND 433E6  // you can set band here directly,e.g. 868E6,915E6
#define SYNC_WORD 0xe4

#define MQTT_HOST_SERVER "192.168.1.250"
#define MQTT_HOST_PORT 1883

#define JSON_SERIALISATION_LIMIT 1024

#define MAX_SUPPORTED_CONNECTED_DEVICES 32

#define NeoPixelPin 12
#define NeoPixelCount 9
#define NeoPixelFlags NEO_GRB + NEO_KHZ400