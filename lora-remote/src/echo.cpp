#include <echo.h>

const bool initiator = true;

unsigned long sendTime;
unsigned long receiveTime;

void sendEcho() {
    if (!initiator) {
        return;
    }
    sendMessage("E");
    sendTime = micros();
}

void receiveEcho(char *message, LoRaClass *lora) {
    if (initiator && message[0] == 'R') {
        receiveTime = micros();
        if (receiveTime < sendTime) {
            Serial.print("Time overflowed, can't use cycle, Sent Time: ");
            Serial.print(sendTime);
            Serial.print(", Received Time: ");  
            Serial.println(receiveTime);
        } else {
            unsigned long timeDelta = receiveTime - sendTime;
            Serial.print(timeDelta);
            int packetRssi = lora->packetRssi();
            Serial.print(",");
            Serial.println(packetRssi);
        }
    } else if (!initiator && message[0] == 'E') {
        sendMessage("R");
    }
}