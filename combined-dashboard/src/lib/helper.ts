import dayjs from "dayjs";
import type { Device } from "./store";
// let duration = require('dayjs/plugin/duration')
import duration from 'dayjs/plugin/duration'
dayjs.extend(duration)

export function getLastPing(device: Device) {
    const ping = device.messageLog.find((message) => message.topic === "PING");
    if (!ping || ping.topic !== "PING") {
        return null;
    }
    return ping;
}

export function getIsConnected(device: Device, key: any = null) {
    const ping = getLastPing(device);
    if (!ping) {
        return false;
    }
    return dayjs().diff(ping.datetime) < dayjs.duration(30, "second").asMilliseconds();
}

export function createGeoJSONCircle(center: [number,number], radiusInKm: number, points: number = 64) {
    let coords = {
        latitude: center[1],
        longitude: center[0]
    };

    let km = radiusInKm;

    let ret = [];
    let distanceX = km/(111.320*Math.cos(coords.latitude*Math.PI/180));
    let distanceY = km/110.574;

    let theta, x, y;
    for(let i=0; i<points; i++) {
        theta = (i/points)*(2*Math.PI);
        x = distanceX*Math.cos(theta);
        y = distanceY*Math.sin(theta);

        ret.push([coords.longitude+x, coords.latitude+y]);
    }
    ret.push(ret[0]);

    return {
        "type": "geojson",
        "data": {
            "type": "FeatureCollection",
            "features": [{
                "type": "Feature",
                "geometry": {
                    "type": "Polygon",
                    "coordinates": [ret]
                }
            }]
        }
    };
};
