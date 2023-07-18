<script lang="ts">
    import { MAPBOX_API_TOKEN, MAPBOX_STYLE } from "$lib/const";
    import { alertTable, devices } from "$lib/store";
    import { Map as MapboxMap, Geocoder, Marker, controls } from "@beyonk/svelte-mapbox";
    import MapCircle from "./MapCircle.svelte";
    import type { Dayjs } from "dayjs";
    import { writable, type Readable, type Writable, derived } from "svelte/store";

    import "../../mapbox.scss";

    const { GeolocateControl, NavigationControl, ScaleControl } = controls;


    export let alertingNode: string;


    function flipLatLon(location: [number, number] | null | undefined) {
        if (!location) return;
        return [location[1], location[0]];
    }

    interface geoLocationPoint {
        lat: number;
        lon: number;
        est_distance: number;
        datetime: Dayjs;
        name: string;
        chipID: string;
    }
    let geolocationPoints: Readable<Map<string,geoLocationPoint>> = derived(alertTable,()=>{
        let points = new Map<string,geoLocationPoint>();
        ($alertTable.get(alertingNode) || []).forEach((location) => {
            const device = $devices.get(location.from);
            if (!device) return;
            if (!device.location) return;

            if (points.has(location.from)) {
                return;
                // if (points.get(location.from)?.datetime?.isBefore(location.datetime)) return;
            }
            points.set(location.from, {
                lat: device.location[0],
                lon: device.location[1],
                est_distance: location.est_distance,
                datetime: location.datetime,
                name: device.name || device.chipID,
                chipID: location.from
            })
        });
        return points;
    });

    function findDeviceLocationAverage() {
        let latSum = 0;
        let lonSum = 0;
        let count = 0;
        ($alertTable.get(alertingNode) || []).forEach((location) => {
            const device = $devices.get(location.from);
            if (!device) return;
            if (!device.location) return;

            latSum += device.location[0];
            lonSum += device.location[1];
            count++;
        });
        return [lonSum / count, latSum / count];
    }

    $: locationAverage = findDeviceLocationAverage();
</script>

<h2>Alert Geolocation: {$devices.get(alertingNode)?.name || "Not Found"}</h2>
<div class="map-container">
    <MapboxMap
        accessToken={MAPBOX_API_TOKEN}
        options={{ style: MAPBOX_STYLE }}
        center={locationAverage}
        zoom={20}
    >
        {#each $geolocationPoints.values() as geolocationPoint}
        {console.log(geolocationPoint.est_distance)}
                <Marker
                    lat={geolocationPoint.lat}
                    lng={geolocationPoint.lon}
                    color="rgb(255,0,0)"
                    label="some marker label"
                    popupClassName="class-name"
                >
                    {geolocationPoint.name}
                    <svelte:fragment slot="popup">
                        <div class="popup">
                            <h3>{geolocationPoint.name}</h3>
                            <p>LatLon: {geolocationPoint.lat.toPrecision(5)}, {geolocationPoint.lon.toPrecision(5)}</p>
                            <p>Distance: {geolocationPoint.est_distance.toPrecision(3)}m</p>
                            <p>Time: {geolocationPoint.datetime?.format("YYYY-MM-DD HH:mm:ss a")}</p>
                        </div>
                    </svelte:fragment>
                </Marker>
                <MapCircle
                    lat={geolocationPoint.lat}
                    lng={geolocationPoint.lon}
                    radius_meters={geolocationPoint.est_distance}
                    chipID={geolocationPoint.chipID}
                />
        {/each}
        <NavigationControl />
        <ScaleControl />
    </MapboxMap>
    <div class="location-estimate-wrapper">
        <table class="location-estimate">
            <thead>
                <tr>
                    <th>Device</th>
                    <th>Location Estimate</th>
                    <th>Datetime</th>
                </tr>
            </thead>
            <tbody>
                {#each $alertTable.get(alertingNode) || [] as geolocationPoint}
                    {@const device = $devices.get(geolocationPoint.from)}
                    <tr>
                        <td>{device?.name}</td>
                        <td>{geolocationPoint.est_distance.toPrecision(3)}m</td>
                        <td>{geolocationPoint.datetime?.format("YYYY-MM-DD HH:mm:ss a")}</td>
                    </tr>
                {/each}
            </tbody>
        </table>
    </div>
</div>

<style lang="scss">
    @use "../../variables.scss" as *;
    .map-container {
        aspect-ratio: 4/3;
        // sometimes mapbox objects don't render as expected; troubleshoot by changing the height/width to px
    }
    .location-estimate-wrapper {
        width: 100%;
        height: 10rem;
        margin-top: 1rem;

        overflow: auto;
    }
    .location-estimate {
        width: 100%;
        border-collapse: collapse;
        th,
        td {
            padding: 0.5rem;
            border: 1px solid $color-bg-0;
        }
        th {
            background: $color-bg-0;
            color: $color-text;
            text-align: left;
            position: sticky;
            top: 0;
        }
        tr:nth-child(even) {
            background: $color-bg-0;
        }
        tr:nth-child(odd) {
            background: $color-bg-1;
        }
    }
    .popup {
        background: $color-bg-0;
        color: $color-text;
        padding: 0.5rem;
        border-radius: 4px;
        border: 1px solid $color-bg-0;
    }
    :global(.mapboxgl-popup .mapboxgl-popup-content) {
        background: $color-bg-0;
        color: $color-text;

    }

    h2 {
        font-size: 1.5rem;
    }
</style>
