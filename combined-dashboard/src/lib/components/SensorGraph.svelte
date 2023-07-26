<script lang="ts">
    import type { Sensor } from '$lib/store';
    import type { ChartData, Point, Chart, Colors } from 'chart.js';
    import { Line } from 'svelte-chartjs';

    export let dataset: Sensor[];
    export let sensorName: string;
    export let deviceid: string;

    let suggestedMax: number = 100;
    let suggestedMin: number = 0;
    let title: string = "";
    let unit: string = "";

    let chartRef: Chart<"line", (number | Point)[], unknown>;

    // if(sensorName == "temperature") {
    //     suggestedMax = 40;
    //     suggestedMin = 0;
    //     title = "Temperature";
    //     unit = "Temperature (°C)";
    // } else if(sensorName == "humidity") {
    //     suggestedMax = 100;
    //     suggestedMin = 0;
    //     title = "Humidity";
    //     unit = "Humidity (%)";
    // } else if(sensorName == "pressure") {
    //     suggestedMax = 1200;
    //     suggestedMin = 800;
    //     title = "Pressure";
    //     unit = "Pressure (hPa)";
    // } else if(sensorName == "dust") {
    //     suggestedMax = 700;
    //     suggestedMin = 0;
    //     title = "Dust";
    //     unit = "Dust (ug/m3)";
    // } else if(sensorName == "ecg") {
    //     suggestedMax = 120;
    //     suggestedMin = 0;
    //     title = "ECG";
    //     unit = "Heart Rate (BPM)";
    // } else if(sensorName == "co2") {
    //     suggestedMax = 700;
    //     suggestedMin = 0;
    //     title = "CO2";
    //     unit = "CO2 (ppm)";
    // } else if(sensorName.startsWith("temp")) {
    //     suggestedMax = 40;
    //     suggestedMin = 0;
    //     title = "Temperature";
    //     unit = "Temperature (°C)";
    // } else {
    //     suggestedMax = 100;
    //     suggestedMin = 0;
    //     title = sensorName;
    //     unit = sensorName;
    // }
</script>


<!-- {#key $dataset}   -->
<Line 
    id={deviceid + "-" + sensorName}
    bind:chart={chartRef}
    data={{
        labels: $dataset.map((d) => d.timestamp),
        datasets: [
            {
                label: sensorName,
                data: $dataset.map((d) => d.value),
                borderColor: "white",
                backgroundColor: "white",
                borderWidth: 1,
                pointRadius: 0,
                pointHoverRadius: 0,
                pointHitRadius: 0,
                pointBorderWidth: 0,
                pointHoverBorderWidth: 0,
                pointHoverBackgroundColor: "white",
                pointHoverBorderColor: "white",
                pointHoverRadius: 0,
                pointHoverBorderWidth: 0,
                pointHitRadius: 0,
                pointRadius: 0,
                pointStyle: "circle",
                tension: 0.1,
            },
        ],
    }}
    width={400}
    height={400}
    options={{
        animation: false,
        plugins: {
            legend: {
                display: false,
            },
            title: {
                display: true,
                text: title,
                position: "top",
                align: "center",
                color: "white",
                font: {
                    size: 20,
                    weight: "normal",
                },
            },
        },
        scales: {
            y: {
                border: {
                    display: true,
                    color: "white",
                },
                beginAtZero: true,
                ticks: {
                    color: "white",
                },
                suggestedMax: suggestedMax,
                suggestedMin: suggestedMin,
                grid: {
                    display: false,
                },
                title: {
                    display: true,
                    text: unit,
                    color: "white",
                    font: {
                        size: 15,
                        weight: "normal",
                    },
                } 
            },
            x: {
                ticks: {
                    display: false,
                },
                grid: {
                    display: false,
                },
                border: {
                    display: true,
                    color: "white",
                },
                title: {
                    display: true,
                    text: "Time",
                    color: "white",
                    font: {
                        size: 15,
                        weight: "normal",
                    },

                }
            }
        },
        responsive: false
        
    }
    }
/>
<!-- {/key} -->
