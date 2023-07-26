import { writable, type Writable } from "svelte/store";
import type { ValidMessage } from "./messages";
import type { Dayjs } from "dayjs";

export type SensorLog = Map<string, Sensor[]>;

export type Sensor = {
    sensorName: string,
    values: [number, Dayjs],
}

export type Device = {
    chipID: string,
    name: string,
    class: "personal" | "station" | "hub",
    location: [number, number] | null,
    messageLog: ValidMessage[],
    sensorLog: SensorLog,
}

export const devices: Writable<Map<string,Device>> = writable(new Map(Object.entries({
    "00CC04C40A24": {
        chipID: "00CC04C40A24",
        name: "Personal Device",
        location: null,
        messageLog: [],
        sensorLog: new Map(),
        class: "personal",
    },
    "5C0A6FA4AE30": {
        chipID: "5C0A6FA4AE30",
        name: "Hub Device",
        location: [-35.68315810486083, 174.32884720522713],
        messageLog: [],
        sensorLog: new Map(),
        class: "hub",
    },
    "E8CD55A4AE30": {
        chipID: "E8CD55A4AE30",
        name: "Weather Station",
        location: [-35.682994814441635, 174.32892167274005],
        messageLog: [],
        sensorLog: new Map(),
        class: "station",
    }
})));

export type DistanceMeasurement = {
    from: string,
    est_distance: number,
    datetime: Dayjs
};
export let alertTable: Writable<Map<string,DistanceMeasurement[]>> = writable(new Map());