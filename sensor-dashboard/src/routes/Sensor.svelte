<script lang="ts">
    import type { ChartData, Point, Chart } from 'chart.js';
    import { Line } from 'svelte-chartjs';
    import type { Writable } from 'svelte/store';

    export let dataset: Writable<ChartData<"line", (number | Point)[], unknown>>;
    export let sensorName: string;
    export let deviceid: string;

    let chartRef: Chart<"line", (number | Point)[], unknown>;

    dataset.subscribe((value) => {
        // console.log(value);
        if (chartRef) {
            // console.log(value)
            chartRef.update("active");
        }
    })
</script>


{#key $dataset}  
<Line 
    id={deviceid + "-" + sensorName}
    bind:chart={chartRef}
    data={$dataset}
    width={400}
    height={400}
    options={{
        animation: false,
        scales: {
            y: {
                beginAtZero: true,
            },
        }
    }}
/>
{/key}
