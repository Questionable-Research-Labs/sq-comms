<svelte:head>
    <title>Weather Station Monitoring</title>
    <meta name="description" content="Weather Station Monitoring">
</svelte:head>

<script lang="ts" >
    // subscribe to mqtt websocket ws://192.168.1.25:8080 
    // and listen to topic "SENSOR"
    import { onMount } from 'svelte'; 
    import Paho from 'paho-mqtt';

    let connected = false;

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

        let client = new Paho.Client("192.168.1.250", 8080, `liveDashboard_${Math.random().toString(36).substring(7)}`);
        client.onConnectionLost = onConnectionLost;
        client.onMessageArrived = onMessageArrived;


        client.connect({ onSuccess: onConnect });
        
        function onConnect() {
            // Once a connection has been made, make a subscription and send a message.
            console.log("Connected to MQTT");
            connected = true;
            client.subscribe("SENSOR");
            client.subscribe("UAB");
        }

        function onConnectionLost(responseObject: { errorCode: number; errorMessage: string; }) {
            if (responseObject.errorCode !== 0) {
                console.log("Disconnected from MQTT: " + responseObject.errorMessage);
                connected = false;
            }

            // retry connection in 5 seconds
            setTimeout(() => {
                client.connect({ onSuccess: onConnect });
            }, 1000);
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
                                backgroundColor: "rgba(139,52,40,0.4)",
                                borderColor: "rgba(158, 67, 39, 1)",
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

                        if(x.datasets[0].data.length > 50) {
                            x.datasets[0].data.shift();
                            x.labels.shift();
                        }
                        
                        return x;
                    });
    
                });

            }

            if(message.topic == "UAB") {
                // UAB messages: 
                // Backpack device: {"sender": "Backpack Outside", "data": [{"kind": "co2", "co2":561},{"kind": "temp", "value":22.00},{"kind": "pressure", "value":100804.00},{"kind": "humidity", "value":36.00}]}
                // Suit Device: {"sender":"Suit", "data":[{"kind":"temp","value":20.30},{"kind":"pressure","value":100691.00},{"kind":"humidity","value":44.00},{"kind":"temp", "value":20.00},{"kind":"co2", "co2":487},{"kind":"ecg","bpm":0}]}
                // Weather Station: {"sender":"Weather Station","data":[{"kind":"temp","value":22.1},{"kind":"pressure","value":1004.50},{"kind":"co2","co2":549},{"kind":"dust","dust":233.98},{"kind":"temp","value":21.2},{"kind":"humidity","value":46.0}]}

                let data = JSON.parse(message.payloadString);

                // Each device should have ID's of UAB Backpack, UAB Suit, or UAB Weather Station

                // add to graphdata
                if(!graphdata[data.sender]) {
                    graphdata[data.sender] = {};
                }

                // rename the second 'temp' from suit to 'temp 2'
                if(data.sender == "Suit" || data.sender == "Backpack Outside" || data.sender == "Weather Station") {
                    let tempCount = 0;
                    data.data.forEach((reading: {kind: string, value: number}) => {
                        if(reading.kind == "temp") {
                            tempCount++;
                            if(tempCount > 1) {
                                reading.kind = "temp 2";
                            }
                        }
                    });
                }


                // add the data to the graphdata array
                data.data.forEach((reading: {kind: string, value: number}) => {
                    if(!graphdata[data.sender][reading.kind]) {
                        graphdata[data.sender][reading.kind] = writable({
                            labels: [""],
                            datasets:[{
                                data:[0], 
                                label: (reading.kind.slice(0, 1).toUpperCase() + reading.kind.slice(1)),
                                fill: true,
                                lineTension: 0.3,
                                backgroundColor: "rgba(139,52,40,0.4)",
                                borderColor: "rgba(158, 67, 39, 1)",
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

                    // update the graph data, reading 'value' doesn't exist for co2 and dust, so just use the second value
                    graphdata[data.sender][reading.kind].update((x)=>{
                        x.datasets[0].data.push(reading.value || reading.co2 || reading.dust || reading.bpm);                        
                        x.labels.push("");
                        if(x.datasets[0].data.length > 50) {
                            x.datasets[0].data.shift();
                            x.labels.shift();
                        }
                        
                        return x;
                    });


                });


            }
        }
    
    })

</script>


<div class="container">
    <h1>Live Sensor Data</h1>
    {#if !connected}
        <h2>Connecting to MQTT...</h2>
    {:else}
        <h2>Connected to MQTT</h2>
    {/if}
    <!-- 1 div for each deviceid, containing each graph from each sensor  -->
    <div class="device-container">
        {#each Object.keys(graphdata) as deviceid}
            <div class="device">
                <h2>Device ID: {deviceid}</h2>
                <div class="graph-flexbox">
                    {#each Object.entries(graphdata[deviceid]) as [sensor, dataset]}
                        <div class="graph">
                            <Sensor dataset={dataset} sensorName={sensor} {deviceid}/>
                        </div>
                    {/each}
                </div>
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

    h1 {
        color: white;
        margin: 20px;
        font-size: 5em;
    }

    h2 {
        color: white;
        margin: 20px;
        font-size: 2em;
    }

    .graph-flexbox {
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: row;
        flex-wrap: wrap;
        gap: 20px;
    }

    .container {
        width: 100%;
        height: 100vh;
        display: flex;
        align-items: center;
        flex-direction: column;
    }

    .device-container {
        width: 100%;
        height: fit-content;
        display: flex;
        justify-content: center;
        align-items: center;
        flex-wrap: wrap;
        flex-direction: row;
    }

    .device {
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: column;
        padding: 20px;
        margin: 20px;
    }

    .graph {
        background-color: #1e1e1ee1;
        border-radius: 10px;
        padding: 10px;
        backdrop-filter: blur(10px);
        border: 1px solid #ffffff;  
        box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.75);
    }

</style>