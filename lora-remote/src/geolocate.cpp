#include <geolocate.h>

#if defined(HAB_SYSTEM)

alertSet currentAlerts[MAX_SUPPORTED_CONNECTED_DEVICES] = {};
int currentAlertsIndex = 0;

void newAlert(const char* alteredChipID, uint32_t packetID, alertNode firstHopNode) {
    Serial.print("New alert data from: ");
    Serial.println(alteredChipID);

    // Check if alert doesn't exist
    int alertIndex = -1;
    for (int i = 0; i < currentAlertsIndex; i++) {
	if (currentAlerts[i].packetID == packetID) {
	    alertIndex = i;
	    break;
	}
    }
    if (alertIndex == -1) {
        if (currentAlertsIndex + 1 >= MAX_SUPPORTED_CONNECTED_DEVICES) {
            // Cycle to beginning of array
            currentAlertsIndex = 0;
        }

        // Alert doesn't exist, add it
        alertSet newAlert = {
            alteredChipID,
            packetID,
            {firstHopNode},
            1
        };

        currentAlerts[currentAlertsIndex] = newAlert;
        alertIndex = currentAlertsIndex;
        currentAlertsIndex++;
    } else {
        // Alert exists, update it
        currentAlerts[alertIndex].rssis[currentAlerts[alertIndex].numRssis] = firstHopNode;
        currentAlerts[alertIndex].numRssis++;
    }

    // Estimate distances
    estimateDistances(currentAlerts[alertIndex]);
}

void estimateDistances(alertSet alert) {
    DynamicJsonDocument distances(1024);
    distances.createNestedArray("distances");
    for (int i = 0; i < alert.numRssis; i++) {
        alertNode node = alert.rssis[i];

        // Calculate using formula 10^((-40-RSSI)/(10*powerConstant))
        float powerConstant = 2.8;
        float distanceSensors = pow(10, ((-40 - node.rssi) / (10 * powerConstant)));


        distances["distances"][i]["from"] = node.chipID;
        distances["distances"][i]["est_distance"] = distanceSensors;
    }
    distances["alerted_chip"] = alert.chipID;
    distances["from"] = chipID;

    // Send distances to server
    char serialisedJSON[1024];
    serializeJson(distances, serialisedJSON, 1024);
    forwardPacket("DISTANCES", serialisedJSON);
}

void processAlert(const char* hops, const char* payload, uint32_t packetID, int rssi) {
    Serial.println("Processing alert");
    Serial.println(payload);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    const char* alteredChipID = doc["from"];
    // const char* msg = doc["msg"];
    bool active = doc["active"];
    #if defined(OUTPUT_NEOPIXEL)
    setAlerting(active);
    #endif
    if (!active) {
        return;
    }

	// [{from: "chipID", rssi: "56.2", order: "1"}, {from: "chipID2", rssi: "5.2", order: "0"}], need to extract 'order 0'
	DynamicJsonDocument hopsDoc(1024);
	deserializeJson(hopsDoc, hops);

	// Default to this node
	alertNode firstHopNode = {
	    chipID,
	    rssi
    };

	// Get number of nodes
	int numNodes = hopsDoc.size();
	for (int i = 0; i < numNodes; i++) {
	    if (hopsDoc[i]["order"] == 0) {
            const char* firstChipID = hopsDoc[i]["from"];
            int firstRssi = hopsDoc[i]["rssi"];

            firstHopNode = {
                firstChipID,
                firstRssi
            };
        }
	}
	newAlert(alteredChipID, packetID, firstHopNode);
}

#endif