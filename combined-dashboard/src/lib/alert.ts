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
            let chipID = message.payload.alerted_chip;
            let existingDistances = alertTable.get(chipID) || [];
            let newDistances = message.payload.distances.map((distance) => {
                return {
                    ...distance,
                    datetime: message.datetime,
                };
            });
            alertTable.set(chipID, [ ...newDistances, ...existingDistances]);
            return alertTable;
        });
    }
}