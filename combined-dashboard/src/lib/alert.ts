import { writable, type Writable } from "svelte/store";
import type { ValidMessage } from "./messages";
import { alertTable } from "./store";


export function processAlertMessages(message: ValidMessage) {
    if (message.topic === "ALERT") {
        alertTable.update((alertTable) => {
            if (message.payload.active) {
                if (!alertTable.has(message.payload.from)) {
                    alertTable.set(message.payload.from, []);
                }
            } else {
                alertTable.delete(message.payload.from);
            }
            return alertTable;
        });
    } else if (message.topic === "DISTANCES") {
        alertTable.update((alertTable) => {
            if (alertTable.has(message.payload.alerted_chip)) {
                alertTable.set(message.payload.alerted_chip, message.payload.distances);
            }
            return alertTable;
        });
    }
}