import { writable, type Writable } from "svelte/store";
import type { ValidMessage } from "./messages";

export type Device = {
    chipID: string,
    name: string,
    class: "personal" | "station" | "hub",
    location: [number, number] | null,
    messageLog: ValidMessage[],
}

export const devices: Writable<Map<string,Device>> = writable(new Map(Object.entries({
    "00CC04C40A24": {
        chipID: "00CC04C40A24",
        name: "Personal Device",
        location: null,
        messageLog: [],
        class: "personal",
    },
    "5C0A6FA4AE30": {
        chipID: "5C0A6FA4AE30",
        name: "Hub Device",
        location: [0,0],
        messageLog: [],
        class: "hub",
    }
})));