<script lang="ts">
    import TechOnGear from "$lib/assets/photos/TechOnGear.jpg?w=600;1920&format=webp";
    import DesignSketch from "$lib/assets/photos/DesignSketch.jpg?w=600;1920&format=webp";
    import SensorTesting from "$lib/assets/photos/SensorTesting.jpg?w=600;1920&format=webp";
    import AlertRunning from "$lib/assets/photos/AlertRunning.jpg?w=600;1920&format=webp";
    import JasperLight from "$lib/assets/photos/JasperLight.jpg?w=600;1920&format=webp";
    import PersonalDeviceSketch from "$lib/assets/photos/PersonalDeviceSketch.jpg?w=600;1920&format=webp";
    import SharedTable from "$lib/assets/photos/SharedTable.jpg?w=600;1920&format=webp";
    import TaineL from "$lib/assets/photos/TaineL.jpg?w=600;1920&format=webp";
    import WhiteBoard from "$lib/assets/photos/WhiteBoard.jpg?w=600;1920&format=webp";

    import Image from "$lib/components/Image.svelte";
    import Gallery from 'svelte-image-gallery/Gallery.svelte';
    import qrlLogo from "$lib/assets/qrl.svg";

    let images = [
        TechOnGear,
        DesignSketch,
        SensorTesting,
        AlertRunning,
        JasperLight,
        PersonalDeviceSketch,
        SharedTable,
        TaineL,
        WhiteBoard,
    ];

    function openImage({detail}: {detail: {class: string, src: string, loading: string, alt: string}}) {
        console.log(images,detail)
        // Regex image ID from class
        let imageID = (detail.class.match(/image-(\d+)/) || [])[1];
        if (imageID) {
            // Open high-res version
            open(images[parseInt(imageID)][1]);
        } else {
            console.log("Couldn't find", detail, imageID);
            // Open low-res as a backup
            open(detail.src);
        }
    }
</script>

<h1>About Signal Quo</h1>
<p>
    Signal Quo is an prototype built by Somewhat Questionable. Our
    team of six claimed a top-place finish at <a
        href="https://ymcachch.org.nz/grapple/"
        target="_blank">Grapple23</a
    >, a week-long maker hackathon, held in
    <a href="https://4c.nz/">YMCA’s 4C Centre</a> in Christchurch, New Zealand.
</p>

<p>
    During the week, we developed a secondary emergency communication network for a Mars colony. Consisting of a main hub, personal transmitters, and environmental monitoring devices, Signal Quo is designed to operate during even severe Martian dust storms. The system utilizes long range (LoRa) technology, and incorporates a novel HopTrace protocol that records each signal's path, enabling geolocation for nodes in distress.
</p>

<p>
    At Signal Quo, we aim to fortify future Martian missions with an empathetic
    approach to safety and efficiency. We journey into a future where Mars isn't
    merely a neighbouring planet, but a second home for humankind. However, this
    isn't a future devoid of challenges. The Martian surface, characterised by
    frequent severe dust storms, embodies an intimidating threat to
    communication - a lifeline for survival on the Red Planet.
</p>

<h2>Who We Are</h2>
<p>
    Our team, aptly named "Somewhat Questionable," is a collective of dedicated
    individuals hailing from different parts of New Zealand.
</p>
<ul>
    <li>
        <span class="name">Jude Wilson</span> and
        <span class="name">Sean Jang</span> were the dynamic design duo of the team. Calling
        Auckland home, they worked in tandem on the physical design of our innovative
        devices, shaping the aesthetics and ergonomics to suit the challenging Martian
        environment.
    </li>
    <li>
        <span class="name">Jasper Miller-Waugh</span> and Taine Reader were our Whangarei
        whizzes who took on the electronics coding. Jasper worked on writing a streamlined
        C++ firmware for the devices and developed the geolocation and HopTrace protocol.
        At the same time, Taine brought his skill set in electronics to create our
        sensor data logger and live sensor display.
    </li>
    <li>
        <span class="name">James Avenell</span> worked his tech magic on the web
        alert dashboard and ensured secure storage for our sensor data.
    </li>
    <li>
        <span class="name">Ara Bartlett</span> helped powered the alert dashboard
        with her skilled electronics work and ensured we got our message across effectively
        with her presentation and design efforts.
    </li>
</ul>

<p>
    Half of our team represents <a href="https://questionable.org.nz/"
        >Questionable Research Labs</a
    >, a makerspace located in Whangarei, underpinning our team's name -
    "Somewhat Questionable".
</p>

<h2>Our Vision</h2>
<p>
    Imagine invaluable human life thriving amidst the daunting Martian dust
    storms, fortified by a resilient communication system - that's the future
    Signal Quo aspires to facilitate.
</p>
<p>
    Out of this vision, the Signal Quo system was born - a mesh network acting
    as a secondary communication system for Martian colonists, featuring a main
    hub, personal transmitters, and environmental monitoring modules. All these
    devices are designed to function meticulously even during severe dust
    storms, making every EVA safer and more efficient.
</p>

<h2>How It Works</h2>
<p>
    Our system utilises the Low Range (LoRa) technology for its radio
    communication protocol - not using the internet-expanded LoRaWAN version
    but the raw LoRa radio, focusing on creating a network directly serving
    the specific needs of a Mars colony.
</p>
<p>
    Our mesh network consists of nodes that connect and work together to
    expand communication coverage. Each node powers our novel HopTrace
    protocol which accurately captures and records all the communication
    hops, allowing geolocation of devices several hops away from the main
    hub.
</p>
<h2>Contact Us</h2>
<p>
    We welcome any inquiries or additional information requests at <a
        href="mailto:signalquo@questionable.org.nz"
        target="_blank">signalquo@questionable.org.nz</a
    >. If you'd like to learn more about the event that sparked our journey,
    visit the official
    <a href="https://ymcachch.org.nz/grapple/" target="_blank">Grapple23</a>
    website.
</p>
<hr />
<Gallery on:click={openImage} hover={true} loading maxColumnWidth={600}>
    {#each images as image, i}
        <img src={image[0]}  class="image-{i}"/>
    {/each}
</Gallery>
<a href="https://questionable.org.nz/" target="_blank" class="qrl-Logo">
    <img src={qrlLogo} alt="Questionable Research Labs Logo" width="256" height="256"/>
</a>
<style lang="scss">
    @use "../../variables.scss" as *;
    h1 {
        font-size: 4rem;
        font-weight: bolder;
        letter-spacing: -0.1rem;
        margin: 0 0 0.2em 0;
        text-align: left;
    }
    h2 {
        font-size: 2rem;
        font-weight: bolder;
        letter-spacing: -0.1rem;
        margin: 1rem 0 0.2em 0;
    }
    p {
        margin: 0 0 1em 0;
    }

    .name {
        font-weight: bolder;
        color: $color-text-selected;
    }
    .body-text {
        background-color: #333;
        box-shadow: 0 0px 1em 3em #333;
    }

    .image-row {
        display: flex;
        flex-direction: row;
        align-items: bottom;
        gap: 1em;
        @media screen and (max-width: $mobile-transition) {
            flex-wrap: wrap;
        }
    }
    :global(img) {
        cursor: pointer;
    }

    .qrl-Logo {

        img {
            display: block;
            margin: 1em auto;
            margin: 5rem auto;
        }
    }
    hr {
        margin: 2rem 0;
    }
</style>
