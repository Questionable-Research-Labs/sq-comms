import dayjs from "dayjs";
import type { Device } from "./store";
// var duration = require('dayjs/plugin/duration')
import duration from 'dayjs/plugin/duration'
dayjs.extend(duration)

export function getLastPing(device: Device) {
    const ping = device.messageLog.find((message) => message.topic === "PING");
    if (!ping) {
        return null;
    }
    return ping;
}

export function getIsConnected(device: Device) {
    const ping = getLastPing(device);
    if (!ping) {
        return false;
    }
    return dayjs().diff(ping.datetime) < dayjs.duration(5, "minute").asMilliseconds();
}