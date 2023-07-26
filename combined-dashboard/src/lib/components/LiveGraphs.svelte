<script lang="ts">
    import { getMetadata } from "$lib/sensorData";
    import { devices } from "$lib/store";
    import SensorGraph from "./SensorGraph.svelte";
    
</script>


<div class="device-container">
    {#each [...$devices.values()].filter((x)=>x.class=="station") as device}
        <div class="device">
            <h2 class="device-name">{device.name} ({device.chipID})</h2>
            <div class="graph-flexbox">
                {#each device.sensorLog as [sensor, dataset]}
                {@const meta = getMetadata(sensor)}
                    <div class="graph">
                        <h3>{meta.title}</h3>
                        <SensorGraph {meta} dataset={dataset} sensorName={sensor} deviceid={device.chipID}/>
                    </div>
                {/each}
            </div>
        </div>
    {/each}
</div>

<style lang="scss">
    @use "../../variables.scss" as *;
    .device-name {
        text-align: center;
        font-weight: bold;
        font-size: 2rem;
    }
    .graph-flexbox {
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: row;
        flex-wrap: wrap;
        gap: 1em;

        .graph {
            border: white solid 2px;
            border-radius: 1rem;
            padding: 1rem;
            background-color: $color-bg-0;
            h3 {
                text-align: center;
            }
        }
    }
    
</style>