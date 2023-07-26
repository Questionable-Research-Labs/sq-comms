import type { ValidMessage } from "./messages";
import { devices } from "./store";

export interface SensorMeta {
    suggestedMax: number,
    suggestedMin: number,
    title: string,
    unit: string,
}

export interface SensorNames {
    [key: string]: SensorMeta
}

export const sensorNames: SensorNames = {
    "temperature": {
        suggestedMax: 40,
        suggestedMin: 0,
        title: "Temperature",
        unit: "Temperature (°C)",
    },
    "humidity": {
        suggestedMax: 100,
        suggestedMin: 0,
        title: "Humidity",
        unit: "Humidity (%)",
    },
    "pressure": {
        suggestedMax: 1200,
        suggestedMin: 800,
        title: "Pressure",
        unit: "Pressure (hPa)",
    },
    "dust": {
        suggestedMax: 700,
        suggestedMin: 0,
        title: "Dust",
        unit: "Dust (ug/m3)",
    },
    "ecg": {
        suggestedMax: 120,
        suggestedMin: 0,
        title: "ECG",
        unit: "Heart Rate (BPM)",
    },
    "co2": {
        suggestedMax: 700,
        suggestedMin: 0,
        title: "CO2",
        unit: "CO2 (ppm)",
    }
};

export function processSensorMessages(message: ValidMessage) {
    // Process sensor messages
    if (message.topic === "SENSOR") {
        message.payload.readings.forEach((reading) => {
            devices.update((devices) => {
                let device = devices.get(message.payload.from);
                if (!device) {
                    return devices;
                }
                let sensor = device.sensorLog.get(reading.sensorName);
                if (!sensor) {
                    device.sensorLog.set(reading.sensorName, [{
                        x: message.datetime.toISOString(),
                        y: reading.value,
                    }]);
                } else {
                    sensor.push({
                        x: message.datetime.toISOString(),
                        y: reading.value,
                    });
                    if (sensor.length > 50) {
                        sensor.shift();
                    }
                }
                return devices;
            });
        });
    }
}

export function getMetadata(sensorName: string) {
    if (sensorName in sensorNames) {
        return sensorNames[sensorName];
    } else if (sensorName.startsWith("temp")) {
        return sensorNames["temperature"];
    } else {
        return {
            suggestedMax: 100,
            suggestedMin: 0,
            title: sensorName,
            unit: sensorName,
        };
    }
}