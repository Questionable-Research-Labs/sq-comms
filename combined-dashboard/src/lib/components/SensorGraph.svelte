<script lang="ts">
    import type { SensorMeta } from '$lib/sensorData';
    import type { Sensor } from '$lib/store';
    import type { ChartData, Colors, Point } from 'chart.js';
    import {Chart,CategoryScale, LinearScale, PointElement, LineElement } from 'chart.js';
    import { Line,  } from 'svelte-chartjs';

    export let dataset: Sensor[];
    export let sensorName: string;
    export let deviceid: string;
    export let meta: SensorMeta;

    Chart.register(CategoryScale);
    Chart.register(LinearScale);
    Chart.register(PointElement);
    Chart.register(LineElement);

    let chartRef: Chart<"line", (number | Point)[], unknown>;

    let windowWidth = window.innerWidth;
    $: isMobile = windowWidth < 800 ? true : false;

</script>

<svelte:body bind:clientWidth={windowWidth}></svelte:body>

<Line 
    id={deviceid + "-" + sensorName}
    bind:chart={chartRef}
    data={{
        datasets: [
            {
                label: sensorName,
                data: dataset,
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
                pointStyle: "circle",
                tension: 0.1,
            },
        ],
    }}
    options={{
        animation: false,
        plugins: {
            legend: {
                display: false,
            },
            title: {
                display: true,
                text: meta.title,
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
                suggestedMax: meta.suggestedMax,
                suggestedMin: meta.suggestedMin,
                grid: {
                    display: false,
                },
                title: {
                    display: true,
                    text: meta.unit,
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
                    display: true,
                    color: "#555"
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
    width={isMobile ? 300 : 400}
    height={isMobile ? 300 : 400}
/>