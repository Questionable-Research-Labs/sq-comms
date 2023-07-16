import type { Message } from 'paho-mqtt';
import { z } from "zod";
import { devices } from './store';
import dayjs, {type Dayjs} from 'dayjs';
import { get } from 'svelte/store';
import { processAlertMessages } from './alert';


// Zod schema
const ValidMessageSchema = z.union([
    z.object({
        topic: z.literal("SENSOR"),
        payload: z.object({
            from: z.string(),
            readings: z.array(z.object({
                sensorName: z.string(),
                value: z.number(),
            })),
        }),
        datetime: z.instanceof(dayjs as unknown as typeof Dayjs),
    }),
    z.object({
        topic: z.literal("ALERT"),
        payload: z.object({
            from: z.string(),
            msg: z.string(),
            active: z.boolean(),
        }),
        datetime: z.instanceof(dayjs as unknown as typeof Dayjs),
    }),
    z.object({
        topic: z.literal("PING"),
        payload: z.object({
            from: z.string(),
            uptime: z.number(),
        }),
        datetime: z.instanceof(dayjs as unknown as typeof Dayjs),
    }),
    z.object({
        topic: z.literal("DISTANCES"),
        payload: z.object({
            from: z.string(),
            alerted_chip: z.string(),
            distances: z.array(z.object({
                from: z.string(),
                est_distance: z.number(),
            })),
        }),
        datetime: z.instanceof(dayjs as unknown as typeof Dayjs),
    }),
]);

export type ValidMessage = z.infer<typeof ValidMessageSchema>;

async function validateMessage(message: Message) {
    try {
        const validMessage = ValidMessageSchema.parse({topic: message.destinationName, payload: JSON.parse(message.payloadString), datetime: dayjs()});
        return validMessage;
    } catch (error) {
        console.error(error);
        return null;
    }
}

export async function newMessage(message: Message) {
    const validMessage = await validateMessage(message);
    if (!validMessage) {
        return;
    }
    console.log("Valid!",validMessage);
    devices.update((devices) => {
        let device = devices.get(validMessage.payload.from);
        if (!device) {
            devices.set(validMessage.payload.from, {
                chipID: validMessage.payload.from,
                name: validMessage.payload.from,
                location: null,
                messageLog: [validMessage],
                class: "station",
            });
        } else {
            device.messageLog.unshift(validMessage);
        }
        return devices;
    });

    // Process alert messages
    processAlertMessages(validMessage);
}