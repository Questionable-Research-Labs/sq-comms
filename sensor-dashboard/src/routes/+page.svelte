<svelte:head>
    <title>Weather Station Monitoring</title>
    <meta name="description" content="Weather Station Monitoring">
</svelte:head>

<script lang="ts" >
    // subscribe to mqtt websocket ws://192.168.1.25:8080 
    // and listen to topic "SENSOR"
    import { onMount } from 'svelte'; 
    import Paho from 'paho-mqtt';

    import {
        Chart as ChartJS,
        Title,
        Tooltip,
        Legend,
        LineElement,
        LinearScale,
        PointElement,
        CategoryScale,
        Filler
    } from 'chart.js';

    import { writable, type Writable } from 'svelte/store';
    import Sensor from './Sensor.svelte';

    ChartJS.register(
        Title,
        Tooltip,
        Legend,
        LineElement,
        LinearScale,
        PointElement,
        CategoryScale,
        Filler
    );
    
    // define graph data list
    type SensorData = {
        [key: string]: Writable<any>;
    };
    type NodeDevices = {
        [key: string]: SensorData;
    };
    let graphdata: NodeDevices= {};

    // graphdata looks like:
    // {
    //     "70CD55A4AE30": {
    //         "temperature": [
    //             23.1,
    //             23.2,
    //             23.3
    //         ]
    //     }
    // }

    onMount(() => {

        let client = new Paho.Client("192.168.1.250", 8080, "clientId");
        client.onConnectionLost = onConnectionLost;
        client.onMessageArrived = onMessageArrived;
        client.connect({ onSuccess: onConnect });
        
        function onConnect() {
            // Once a connection has been made, make a subscription and send a message.
            console.log("Connected to MQTT");
            client.subscribe("#");
        }

        function onConnectionLost(responseObject: { errorCode: number; errorMessage: string; }) {
            if (responseObject.errorCode !== 0) {
                console.log("Disconnected from MQTT: " + responseObject.errorMessage);
            }

            // retry connection in 5 seconds
            setTimeout(() => {
                client.connect({ onSuccess: onConnect });
            }, 5000);
        }

        function onMessageArrived(message: { topic: string; payloadString: string; }) {
            console.log("Message: " + message.topic + ": " + message.payloadString);
            
            if(message.topic == "SENSOR") {
                // sensor messages look like: {"from":"70CD55A4AE30","readings":[{"sensorName":"temperature","value":23.10000038}]}
                let data = JSON.parse(message.payloadString);
                
                // check what device ID is sending the data, log the data under ID -> sensor type (temp, etc) -> value list
                if(!graphdata[data.from]) {
                    graphdata[data.from] = {};
                }

                // add the data to the graphdata array
                data.readings.forEach((reading: {sensorName: string, value: number}) => {
                    if(!graphdata[data.from][reading.sensorName]) {
                        graphdata[data.from][reading.sensorName] = writable({
                            labels: [""],
                            datasets:[{
                                data:[0], 
                                label: (reading.sensorName.slice(0, 1).toUpperCase() + reading.sensorName.slice(1)),
                                fill: true,
                                lineTension: 0.3,
                                backgroundColor: "rgba(225, 204,230, .3)",
                                borderColor: "rgb(205, 130, 158)",
                                borderCapStyle: "butt",
                                borderDash: [],
                                borderDashOffset: 0.0,
                                borderJoinStyle: "miter",
                                pointBorderColor: "rgb(205, 130,1 58)",
                                pointBackgroundColor: "rgb(255, 255, 255)",
                                pointBorderWidth: 10,
                                pointHoverRadius: 5,
                                pointHoverBackgroundColor: "rgb(0, 0, 0)",
                                pointHoverBorderColor: "rgba(220, 220, 220,1)",
                                pointHoverBorderWidth: 2,
                                pointRadius: 0,
                                pointHitRadius: 10,
                            }]
                        });
                    }

                    // update the graph data
                    graphdata[data.from][reading.sensorName].update((x)=>{
                        x.datasets[0].data.push(reading.value);
                        x.labels.push("");

                        if(x.datasets[0].data.length > 10) {
                            x.datasets[0].data.shift();
                            x.labels.shift();
                        }
                        
                        return x;
                    });
    
                });

            }
        }

        // make up random data for testing, send it every second
        setInterval(() => {
            if (client.isConnected()) {
                let data = {
                from: "TEST_DEVICE",
                readings: [
                    {
                        sensorName: "temperature",
                        value: Math.random() * 100
                    },
                    {
                        sensorName: "humidity",
                        value: Math.random() * 100
                    }
                ]}
                client.send("SENSOR", JSON.stringify(data));
            } else {
                console.log("Not connected to MQTT");
            }

            if (client.isConnected()) {
                let data = {
                from: "TEST_DEVICE_2",
                readings: [
                    {
                        sensorName: "temperature",
                        value: Math.random() * 100
                    },
                    {
                        sensorName: "humidity",
                        value: Math.random() * 100
                    }
                ]}
                client.send("SENSOR", JSON.stringify(data));
            } else {
                console.log("Not connected to MQTT");
            }
        }, 1000);
    
    })

</script>


<div class="container">
    <h1>Live Sensor Data</h1>
    <!-- 1 div for each deviceid, containing each graph from each sensor  -->
    <div class="device-container">
        {#each Object.keys(graphdata) as deviceid}
            <div class="device">
                <h2>{deviceid}</h2>
                {#each Object.entries(graphdata[deviceid]) as [sensor, dataset]}
                    <Sensor dataset={dataset} sensorName={sensor} {deviceid}/>
                {/each}
            </div>
        {/each}
    </div>
</div>


<style lang="scss">

    @import url('https://fonts.googleapis.com/css2?family=Roboto');
    :global(html) {
        font-family: 'Roboto', sans-serif;
    }

    * {
        margin: 0;
        padding: 0; 
        box-sizing: border-box;
    }

    .container {
        width: 100%;
        height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: column;
        background-color: grey;
    }

    .device-container {
        width: 100%;
        height: 100%;
        display: flex;
        justify-content: center;
        align-items: center;
        flex-wrap: wrap;
    }

    .device {
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: column;
        background-color: white;
        border-radius: 10px;
        padding: 20px;
        margin: 20px;
    }

</style>