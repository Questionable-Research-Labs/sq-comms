<script lang="ts">
    import dayjs from "dayjs";
    import relativeTime from "dayjs/plugin/relativeTime";

    import { getLastPing, getIsConnected } from "$lib/helper";
    import HubIcon from "$lib/icons/HubIcon.svelte";
    import PersonalIcon from "$lib/icons/PersonalIcon.svelte";
    import StationIcon from "$lib/icons/StationIcon.svelte";
    import { alertTable, devices, type Device } from "$lib/store";
    import { getContext, onDestroy } from "svelte";
    import GeolocationMap from "./GeolocationMap.svelte";
    import DeviceMap from "./DeviceMap.svelte";

    const { open } = getContext("simple-modal") as any;

    dayjs.extend(relativeTime);

    let timerKey = 0;
    let timer = setInterval(() => {
        timerKey++;
    }, 0.1);

    onDestroy(() => {
        clearInterval(timer);
    });

    function openAlertMap(device: Device) {
        if ($alertTable.has(device.chipID)) {
            open(GeolocationMap, { alertingNode: device.chipID });
        } else if (device.location) {
            open(DeviceMap);
        }
    }
</script>

<!-- <div class="title"><h2>Connected Nodes</h2></div> -->
<div class="device-container">
    {#each $devices.values() as device}
        {@const lastPing = getLastPing(device)}
        {@const isConnected = getIsConnected(device)}
        {@const lastPingTime = lastPing?.datetime}
        {@const bootTime = lastPingTime?.subtract(
            dayjs.duration(lastPing?.payload.uptime || 0, "milliseconds")
        )}
        {@const lastPingDuration = dayjs.duration(dayjs().diff(lastPingTime))}
        {#key getIsConnected(device, timerKey)}
            <div class="device-config-wrapper">
                <div class="device-config-background">
                    {#if device.class == "hub"}
                        <HubIcon />
                    {:else if device.class == "station"}
                        <StationIcon />
                    {:else}
                        <PersonalIcon />
                    {/if}
                </div>
                <div
                    class="device-config"
                    class:connected={isConnected && lastPing}
                    class:alerting={$alertTable.has(device.chipID)}
                >
                    <h3 title={device.chipID}>
                        {device.name}
                    </h3>
                    <div class="device-config-item">
                        <span class="property">Message Count:</span>
                        <span class="value">{device.messageLog.length}</span>
                    </div>
                    <div class="device-config-item">
                        <span class="property">Last Ping:</span>
                        <span
                            class="value"
                            title={lastPingTime?.toISOString() || "N/A"}
                        >
                            {#key timerKey}
                                {#if lastPingDuration}
                                    {Math.round(lastPingDuration.asSeconds())}s
                                {:else}
                                    N/A
                                {/if}
                            {/key}
                        </span>
                    </div>
                    <div class="device-config-item">
                        <span class="property">Uptime:</span>
                        <span class="value">
                            {#if bootTime && isConnected}
                                <!-- If connected, be positive and show uptime ticking up between pings -->
                                {dayjs
                                    .duration(dayjs().diff(bootTime))
                                    .format("HH:mm:ss")}
                            {:else if bootTime && !isConnected && lastPingTime}
                                <!-- If not connected, show definite uptime -->
                                {dayjs
                                    .duration(lastPingTime.diff(bootTime))
                                    .format("HH:mm:ss")}+
                            {:else}
                                N/A
                            {/if}
                        </span>
                    </div>
                    {#if device.location}
                        <button
                            class="geolocation-map location"
                            on:click={() => {
                                openAlertMap(device);
                            }}
                        >
                            Device Map
                        </button>
                    {:else}
                        <button
                            class="geolocation-map"
                            on:click={() => {
                                openAlertMap(device);
                            }}
                            disabled={!$alertTable.has(device.chipID)}
                        >
                            Alert Geolocation Map
                        </button>
                    {/if}
                </div>
            </div>
        {/key}
    {/each}
</div>

<style lang="scss">
    @use "../../variables.scss" as *;
    // .title {
    //     display: flex;
    //     justify-content: center;
    //     margin: 0;
    //     h2 {
    //         padding: 0 0.2rem 0.1rem 0.2rem;
    //         border-bottom: #eee solid 2px;
    //         font-size: 1.5rem;
    //     }
    // }
    .device-container {
        display: grid;
        grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
        gap: 1em;
        flex: 1 0 21%; /* explanation below */
        .device-config-wrapper {
            $device-config-border-width: 4px;
            $device-config-padding: 1em;
            position: relative;
            .device-config-background {
                border-radius:  $device-config-border-width * 2;
                position: absolute;
                top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                z-index: -1;
                background-color: $color-bg-0;
                padding: $device-config-padding;

                :global(svg) {
                    position: absolute;
                    bottom: calc($device-config-padding + $device-config-border-width);
                    right: calc($device-config-padding + $device-config-border-width);
                    height: calc(100% - (($device-config-padding +  $device-config-border-width) * 2));
                    fill: #444;
                }
            }
            .device-config {
                border: $color-bg-1 solid  $device-config-border-width;
                border-radius:  $device-config-border-width * 2;
                padding: 1em;
                h3 {
                    margin: 0;
                    margin-bottom: 0.5em;
                }

                .device-config-item {
                    display: flex;
                    flex-direction: row;
                    justify-content: left;
                    align-items: center;
                    padding: 0.1em 0 0.1em 0;

                    // border-top: 1px solid #eee;
                    // border-top-style: dashed;

                    .property {
                        margin-right: 0.5em;
                    }
                }
                .value {
                    background-color: #222;
                    padding: 0.2em 0.25em;
                    border-radius: 4px;
                    font-family: $font-mono;
                    font-size: 0.9rem;
                    font-weight: 100;
                }

                &.connected {
                    border-color: $color-success-neutral;
                }
                &.alerting {
                    border-color: $color-error-neutral;
                    animation: alert 0.5s ease-in-out infinite alternate;

                    @keyframes alert {
                        0% {
                            border-color: $color-error-neutral;
                            box-shadow: 0 0 0 0 $color-error-neutral;
                        }
                        100% {
                            border-color: $color-error-bright;
                            box-shadow: 0 0 10px 4px $color-error-bright;
                        }
                    }
                    cursor: pointer;
                }
                .geolocation-map {
                    $button-padding-size: 0.5em;
                    $border-size: 4px;
                    margin-top: $button-padding-size;
                    padding: calc($button-padding-size - ($border-size / 2));
                    border-radius: $button-padding-size;
                    border: none;
                    background-color: $color-bg-1;
                    color: $color-text;
                    font-size: 1rem;
                    font-weight: 100;
                    cursor: pointer;
                    transition: all 0.2s ease-in-out;
                    border: $color-text-selected solid $border-size;
                    &:disabled {
                        background-color: $color-bg-1;
                        color: $color-text-disabled;
                        cursor: default;
                        border: $color-bg-1 solid $border-size;
                    }
                    &:hover:not(:disabled) {
                        // background-color: $color-bg-0;
                        background-color: $color-text-selected;
                    }
                    &.location {
                        border-color: $color-bg-1;
                        &:hover {
                            border: $color-text-selected solid $border-size;
                        }
                    }
                }
            }
        }
    }
</style>
