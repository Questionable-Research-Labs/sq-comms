import type { Message } from 'paho-mqtt';
import { z } from "zod";
import { devices } from './store';
import { get } from 'svelte/store';


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
    }),
    z.object({
        topic: z.literal("ALERT"),
        payload: z.object({
            from: z.string(),
            msg: z.string(),
            active: z.boolean(),
        }),
    }),
    z.object({
        topic: z.literal("PING"),
        payload: z.object({
            from: z.string(),
            uptime: z.number(),
        }),
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
    }),
]);

export type ValidMessage = z.infer<typeof ValidMessageSchema>;

async function validateMessage(message: Message) {
    try {
        const validMessage = ValidMessageSchema.parse(JSON.parse(message.payloadString));
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
                location: [0, 0],
                messageLog: [validMessage],
            });
        } else {
            device.messageLog.push(validMessage);
        }
        return devices;
    });
}