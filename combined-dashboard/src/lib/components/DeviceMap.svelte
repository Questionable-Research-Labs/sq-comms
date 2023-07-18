<script lang="ts">
    import { MAPBOX_API_TOKEN, MAPBOX_STYLE } from "$lib/const";
    import { getLastPing } from "$lib/helper";
    import { devices, type Device } from "$lib/store";
    import {
        Map as MapboxMap,
        Marker,
        controls
    } from "@beyonk/svelte-mapbox";

    import "../../mapbox.scss";


    const { NavigationControl, ScaleControl } = controls;


    function findDeviceLocationAverage(devices: Map<string,Device>) {
        let latSum = 0;
        let lonSum = 0;
        let count = 0;
        for (let device of devices.values()) {
            console.log(device);
            if (!device.location) continue;

            latSum += device.location[0];
            lonSum += device.location[1];
            count++;
        }
        return [lonSum / count, latSum / count];
    }

    $: locationAverage = findDeviceLocationAverage($devices);
    $: console.log("test",locationAverage);
</script>

<h2>Device Map</h2>

<div class="map-container">
    <MapboxMap
        accessToken={MAPBOX_API_TOKEN}
        options={{ style: MAPBOX_STYLE }}
        center={locationAverage}
        zoom={20}
    >
        {#each $devices.values() as geolocationPoint}
            {#if geolocationPoint.location}
                <Marker
                    lat={geolocationPoint.location[0]}
                    lng={geolocationPoint.location[1]}
                    label="some marker label"
                    popupClassName="class-name"
                >
                    <svelte:fragment slot="popup">
                        <div class="popup">
                            <h3>{geolocationPoint.name}</h3>
                            <p>
                                LatLon:
                                {geolocationPoint.location[0].toPrecision(5)},
                                {geolocationPoint.location[1].toPrecision(5)}
                            </p>
                            <p>
                                Last Ping: {getLastPing(geolocationPoint)?.datetime?.format("HH:mm:ss")}
                            </p>
                            <p>
                                ChipID: {geolocationPoint.chipID}
                            </p>
                        </div>
                    </svelte:fragment>
                </Marker>
            {/if}
        {/each}
        <NavigationControl />
        <ScaleControl />
    </MapboxMap>
</div>

<style lang="scss">
    @use "../../variables.scss" as *;
    .map-container {
        aspect-ratio: 4/3;
        // sometimes mapbox objects don't render as expected; troubleshoot by changing the height/width to px
    }
    .popup {
        background: $color-bg-0;
        color: $color-text;
        padding: 0 0.5rem;
        border-radius: 4px;
        border: 1px solid $color-bg-0;
        h3 {
            margin: 0 0 0.5rem 0;
            font-size: 1.3rem;
        }
        p {
            margin: 0.2rem 0 0 0;
        }
    }
    :global(.mapboxgl-popup .mapboxgl-popup-content) {
        background: $color-bg-0;
        color: $color-text;

    }

    h2 {
        font-size: 1.5rem;
    }
</style>
