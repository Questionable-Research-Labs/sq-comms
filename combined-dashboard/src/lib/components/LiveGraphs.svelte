<script lang="ts">
    import { devices } from "$lib/store";
    import SensorGraph from "./SensorGraph.svelte";
    
    
</script>

Sensor live graphs here

<div class="device-container">
    {#each [...$devices.values()].filter((x)=>{x.class="station"}) as device}
        <div class="device">
            <h2>Device ID: {device.chipID}</h2>
            <div class="graph-flexbox">
                {#each device.sensorLog as [sensor, dataset]}
                    <div class="graph">
                        <SensorGraph dataset={dataset} sensorName={sensor} deviceid={device.chipID}/>
                    </div>
                {/each}
            </div>
        </div>
    {/each}
</div>