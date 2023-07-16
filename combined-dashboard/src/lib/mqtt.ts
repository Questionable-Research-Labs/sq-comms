import Paho, { type MQTTError } from 'paho-mqtt';
import {MQTT_SERVER, MQTT_PORT} from "$lib/const";
import { newMessage } from './messages';

export let mqttConnected: boolean = false;

export async function mqttConnect() {
    let client = new Paho.Client(MQTT_SERVER, MQTT_PORT, `liveDashboard_${Math.random().toString(36).substring(7)}`);

    function onConnect() {
        // Once a connection has been made, make a subscription and send a message.
        console.log("Connected to MQTT");
        mqttConnected = true;
        // Subscribe to all topics
        client.subscribe("#");
    }

    function onConnectionLost(mqttError: MQTTError) {
        console.log(`MQTT connection lost: ${mqttError.errorMessage}`);
        mqttConnected = false;
        setTimeout(() => {
            client.connect({ onSuccess: onConnect });
        }, 1000);
    }
    
    function onMessageArrived(message: Paho.Message) {
        console.log(`MQTT message received: ${message.payloadString}`);

        // Escape callback hell
        setTimeout(()=>{newMessage(message)})
    }

    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    client.connect({ onSuccess: onConnect, useSSL: true });

}
