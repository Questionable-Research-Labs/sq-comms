#include <geolocate.h>

#if defined(HAB_SYSTEM)

alertSet currentAlerts[MAX_SUPPORTED_CONNECTED_DEVICES] = {0};
int currentAlertsIndex = 0;

void newAlert(const char* chipID, alertNode firstHopNode) {
    Serial.println("New alert");
    
    // Check if alert doesn't exist
    int alertIndex = -1;
    for (int i = 0; i < MAX_SUPPORTED_CONNECTED_DEVICES; i++) {
        if (strcmp(currentAlerts[i].chipID, chipID) == 0) {
            alertIndex = i;
            break;
        }
    }
    if (alertIndex == -1) {
        if (currentAlertsIndex+1 >= MAX_SUPPORTED_CONNECTED_DEVICES) {
            // Cycle to beginning of array
            currentAlertsIndex = 0;
        }

        // Alert doesn't exist, add it
        alertSet newAlert = {
            chipID,
            {firstHopNode},
            1
        };
        currentAlerts[currentAlertsIndex] = newAlert;
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
    Serial.println("Estimating distances");
    DynamicJsonDocument distances(1024);
    distances.createNestedArray("distances");
    for (int i = 0; i < alert.numRssis; i++) {
        alertNode node = alert.rssis[i];

        float distanceSensors = node.rssi+25;

        distances["distances"][i]["from"] = node.chipID;
        distances["distances"][i]["est_distance"] = distanceSensors;
    }
    distances["alerted_chip"] = alert.chipID;

    // Send distances to server
    char serialisedJSON[1024];
    serializeJson(distances, serialisedJSON, 1024);
    forwardPacket("DISTANCES", serialisedJSON);
}

void processAlert(const char* hops,const char* payload, int rssi) {
    Serial.println("Processing alert");
    Serial.println(payload);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    const char* chipID = doc["from"];
    // const char* msg = doc["msg"];
    bool active = doc["active"];

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
    

    if (active) {
        newAlert(chipID, firstHopNode);
    }
}

#endif