<script lang="ts">
    import Logo from "$lib/assets/logo.png?as=url";
    import { alertTable, devices } from "$lib/store";
    import { onDestroy } from "svelte";
    import { tweened } from "svelte/motion";

    const backgroundColorAnimationSpeed = 500;
    let backgroundColor = tweened([78,71,129], {
        duration: backgroundColorAnimationSpeed,
    });
    let pulse = false;
    let alerting = false;
    function pulseUpdate() {
        if (alerting) {
            console.log(pulse);
            if (pulse) {
                backgroundColor.set([129, 71, 71]);
            } else {
                backgroundColor.set([176, 40, 40]);
            }
        } else {
            backgroundColor.set([78,71,129]);
        }
        pulse = !pulse;
    }

    let lifetime = setInterval(pulseUpdate, backgroundColorAnimationSpeed);

    // Overcomplicated method to sync pulses with node page
    let subscription = alertTable.subscribe((table)=>{
        if (table.size > 0) {
            if (!alerting) {
                alerting = true;
                //Immediately restart animation to sync with the next pulse
                clearInterval(lifetime);
                lifetime = setInterval(pulseUpdate, backgroundColorAnimationSpeed);
                pulse = false;
                pulseUpdate();
            } else {
                alerting = true;
            }
        } else {
            alerting = false;
        }
    })
    
    onDestroy(() => {
        clearInterval(lifetime);
        subscription();
    });
</script>
<div class="header-container" style="--backgroundColor: {$backgroundColor.join(", ")}">
    <div class="logo-container">
        <img src={Logo[0]} alt="Signal Quo Logo">
        <span>SIGNAL QUO</span>
    </div>
    
    
    <div class="hills-container">
        <svg
            width="100%"
            viewBox="0 0 1900 450"
            fill="none"
            xmlns="http://www.w3.org/2000/svg"
        >
            <path
                d="M0 92.7374C0 92.7374 198.075 87.9209 334 124.737C474.299 162.739 544.583 284.762 689 301.237C882.852 323.353 1020.5 280.237 1205 209.737C1363.04 149.348 1483.6 10.5397 1652.5 0.737366C1766.55 -5.88151 1900 34.2374 1900 34.2374V440.237H1263.5H848.5H413.5H0V92.7374Z"
                fill="#9E4327"
            />
        </svg>
        <h1 class="nodes-online-label">nodes online</h1>
        <div class="hill-text" id="num">{$devices.size}</div>
        <svg
            width="100%"
            viewBox="0 0 1900 450"
            fill="none"
            xmlns="http://www.w3.org/2000/svg"
        >
            <path
                d="M1424.5 359.5C1520.88 322.111 1627.36 224.143 1727.5 198.5C1793.36 181.635 1900 175 1900 175V450H1470H1167C1167 450 1211.51 442.129 1424.5 359.5Z"
                fill="#943B27"
            />
            <path
                d="M1415.5 405.5C1509.18 361.783 1630.36 266.643 1730.5 241C1796.36 224.135 1900 215.5 1900 215.5V450H1469.5H1275C1275 450 1370 426.734 1415.5 405.5Z"
                fill="#8B3428"
            />
            <path
                d="M0 241.5C0 241.5 21 219.5 68 207C113.621 194.867 113.405 195 154 195C205 195 230.5 253 347.5 253C444.5 253 497.344 348.792 602.5 376.5C695.832 401.093 826.645 327.899 923 333.5C1020.27 339.154 1034.94 276.086 1127 308C1202 334 1336 450 1336 450H426H0V241.5Z"
                fill="#903927"
            />
            <path
                d="M0 286.5C0 286.5 21 264.5 68 252C113.621 239.867 113.405 240 154 240C205 240 230.5 298 347.5 298C444.5 298 497.344 393.792 602.5 421.5C695.832 446.093 826.645 372.899 923 378.5C1020.27 384.154 1034.94 321.086 1127 353C1202 379 1264 449.763 1264 449.763H424.5H0V286.5Z"
                fill="#8B3428"
                id="mid-point"
            />
        </svg>
    </div>
    
</div>

<style lang="scss">
    @use "sass:math";

    .header-container {
        background: radial-gradient(farthest-corner at top,rgba(var(--backgroundColor),1) 0%, rgba(44,44,44,1) 80%);
        overflow: clip;
        border-bottom: white solid 4px;
    }

    .hills-container {
        --node-count-size: 12rem;
        --node-label-size: 2rem;

        $node-count-position: percentage(math.div(232.92, 1403.7));
        $aspect-ratio: math.div(1900, 450) *0.9;
        $node-label-spacing: 1rem;

        position: relative;
        margin-top:  $node-label-spacing;
        aspect-ratio: $aspect-ratio;
        min-height: 140px;
        left: 50%;
        transform: translateX(-50%);

        svg {
            position: absolute;
            bottom: 0;
            transform: scaleY(0.75) translateY(1.3333333333);
        }

        .hill-text {
            position: absolute;
            bottom: $node-count-position;
            left: 50%;
            width: var(--node-count-size);
            height: var(--node-count-size);
            line-height: var(--node-count-size);
            text-align: center;
            color: white;
            background-color: #9787ff;
            border: 4px solid #8078c2;
            border-radius: 50%;
            font-size: calc(var(--node-count-size) - 2rem);
            text-align: center;
            transform: translate(-50%, 0);
        }

        .nodes-online-label {
            position: absolute;
            bottom: calc($node-count-position + var(--node-count-size));
            z-index: 1;
            width: 100%;
            text-align: center;
            font-size: var(--node-label-size);
            text-align: center;
            text-transform: uppercase;
            font-weight: bold;
            margin: 0;
            margin-bottom:  $node-label-spacing;
        }

        @media screen and (max-width: 1100px) {
            --node-count-size: 8rem;
            --node-label-size: 1.5rem;
        }
        @media screen and (max-width: 800px) {
            --node-count-size: 4rem;
            --node-label-size: 1rem;
        }
    }

    .logo-container {
        $logo-height: 4rem;
        
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: center;
        padding-top: calc($logo-height/3);
        * {
            height: $logo-height;
            line-height: $logo-height;
            font-weight: bold;
            font-size: $logo-height * 0.5;
        }
    }
</style>
