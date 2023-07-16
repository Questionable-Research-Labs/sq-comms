<script lang="ts">
    import dayjs from 'dayjs';
    import relativeTime from 'dayjs/plugin/relativeTime';

    import { getLastPing, getIsConnected } from "$lib/helper";
    import HubIcon from "$lib/icons/HubIcon.svelte";
    import PersonalIcon from "$lib/icons/PersonalIcon.svelte";
    import StationIcon from "$lib/icons/StationIcon.svelte";
    import { devices } from "$lib/store";
    import { onDestroy } from 'svelte';

    dayjs.extend(relativeTime);

    let timerKey = 0;
    let timer = setInterval(() => {
        timerKey++;
    }, 1000);

    onDestroy(() => {
        clearInterval(timer);
    });
</script>
<!-- <div class="title"><h2>Connected Nodes</h2></div> -->
<div class="device-container">
    {#each $devices.values() as device}
    {@const lastPing = getLastPing(device)}
    {@const isConnected = getIsConnected(device)}
        <div class="device-config" class:connected={isConnected && lastPing} class:disconnected={!isConnected  && lastPing}>
            {#if device.class == "hub"}
                <HubIcon />
            {:else if device.class == "station"}
                <StationIcon />
            {:else}
                <PersonalIcon />
            {/if}
            <h3 title={device.chipID}>{device.name}</h3>
            <div class="device-config-item">
                <span class="property">Message Count:</span>
                <span class="value">{device.messageLog.length}</span>
            </div>
            <div class="device-config-item">
                <span class="property">Last Ping:</span>
                <span class="value" title={lastPing?.datetime.toISOString() || "N/A"}>
                    {#key timerKey}
                        {lastPing?.datetime.fromNow(false) || "N/A"}
                    {/key}
                </span>
            </div>
        </div>
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
        .device-config {
            position: relative;
            border: $color-bg-1 solid 4px;
            border-radius: 4px * 2;
            padding: 1em;
            h3 {
                margin: 0;
                margin-bottom: 0.5em;
            }
            :global(svg) {
                position: absolute;
                bottom: 0;
                left: 0;
                height: 100%;
                fill: #444;
                z-index: -1;
            }
            .device-config-item {
                display: flex;
                flex-direction: row;
                justify-content: space-between;
                align-items: center;
                padding: 0.15em 0 0.2em 0;
                
                border-top: 1px solid #eee;
                border-top-style: dashed;
                
                .value {
                    background-color: #222;
                    padding: 0.2em 0.25em;
                    border-radius: 4px;
                    font-family: $font-mono;
                }
                .property {
                    margin-right: 0.5em;
                }
            }

            &.connected {
                border-color: $color-success-neutral;
            }
            &.disconnected {
                border-color: $color-error-neutral;
            }
        }
    }
</style>
