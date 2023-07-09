#include <alert.h>

void sendAlert() {
    DynamicJsonDocument doc(1024);

    doc["from"] = "ChipAddrHere";
    doc["msg"] = "test alert";
    doc["class"] = "personal";


    char serialisedJSON[1024];
    serializeJson(doc, serialisedJSON, 1024);
    sendMessage("ALERT", serialisedJSON);
}