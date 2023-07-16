<script lang="ts">
    import { MAPBOX_API_TOKEN } from "$lib/const";
    import { alertTable, devices } from "$lib/store";
    import { Map, Geocoder, Marker, controls } from "@beyonk/svelte-mapbox";
    import MapCircle from "./MapCircle.svelte";
    const { GeolocateControl, NavigationControl, ScaleControl } = controls;


    export let alertingNode: string;

    let fitBounds: (bbox: any, data?: {}) => void;

    function flipLatLon(location: [number, number] | null | undefined) {
        if (!location) return;
        return [location[1], location[0]];
    }

    // $: (fitBounds ? ()=>{
    //     console.log("testr")
    //     let bounds = ($alertTable.get(alertingNode) || []).map((location) => {
    //         const device = $devices.get(location.from);
    //         console.log(device, location)
    //         return flipLatLon(device?.location);
    //     }).filter((location) => location);
    //     console.log(bounds);
    //     fitBounds(bounds);
    // } : ()=>{})();

    function findDeviceLocationAverage() {
        let locations = ($alertTable.get(alertingNode) || []).map(
            (location) => {
                const device = $devices.get(location.from);
                return flipLatLon(device?.location);
            }
        );
        let latSum = 0;
        let lonSum = 0;
        locations.forEach((location) => {
            if (!location) return;
            latSum += location[0];
            lonSum += location[1];
        });
        return [latSum / locations.length, lonSum / locations.length];
    }

    $: locationAverage = findDeviceLocationAverage();
</script>

<h2>Alert Geolocation: {$devices.get(alertingNode)?.name || "Not Found"}</h2>
<div class="map-container">
    <Map
        accessToken={MAPBOX_API_TOKEN}
        options={{ style: "mapbox://styles/mapbox/dark-v10" }}
        bind:fitBounds
        center={locationAverage}
        zoom={20}
    >
        {#each $alertTable.get(alertingNode) || [] as geolocationPoint}
            {@const device = $devices.get(geolocationPoint.from)}
            {#if device?.location}
                <Marker
                    lat={device?.location[0]}
                    lng={device?.location[1]}
                    color="rgb(255,0,0)"
                    label="some marker label"
                    popupClassName="class-name"
                >
                    {device.name}
                    <svelte:fragment slot="popup">
                        <div class="popup">
                            <h3>{device.name}</h3>
                            <p>LatLon: {device.location[0].toPrecision(5)}, {device.location[1].toPrecision(5)}</p>
                            <p>Distance: {geolocationPoint.est_distance.toPrecision(3)}m</p>
                            <p>Time: {geolocationPoint.datetime?.format("YYYY-MM-DD HH:mm:ss a")}</p>
                        </div>
                    </svelte:fragment>
                </Marker>
                <MapCircle
                    lat={device?.location[0]}
                    lng={device?.location[1]}
                    radius_meters={geolocationPoint.est_distance}
                />
            {/if}
        {/each}
        <NavigationControl />
        <ScaleControl />
    </Map>
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
