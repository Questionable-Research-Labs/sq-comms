import { writable, type Writable } from "svelte/store";
import type { ValidMessage } from "./messages";

export type Device = {
    chipID: string,
    name: string,
    location: [number, number],
    messageLog: ValidMessage[],
}

export const devices: Writable<Map<string,Device>> = writable(new Map())