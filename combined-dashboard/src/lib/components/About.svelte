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
    import WeatherStationBox from "$lib/assets/photos/WeatherStationBox.jpg?w=600;1920&format=webp";
    import JudeDemoPersonalDevice from "$lib/assets/photos/JudeDemoPersonalDevice.jpg?w=600;1920&format=webp";
    import SignalQuoShirtLogo from "$lib/assets/photos/SignalQuoShirtLogo.jpg?w=600;1920&format=webp";

    import {
        GalleryThumbnail,
        GalleryImage,
        LightboxGallery
    } from 'svelte-lightbox';
    import qrlLogo from "$lib/assets/qrl.svg";

    interface Image {
        title: string;
        src: string[];
    }
    let images: Image[] = [
        {
            title: "Chaos of developing sensor modules",
            src: TechOnGear
        },
        {
            title: "Sean's design sketches",
            src: DesignSketch
        },
        {
            title: "Taine testing the dust sensor",
            src: SensorTesting
        },
        {
            title: "Main hub alert running",
            src: AlertRunning
        },
        {
            title: "Jasper working on the main hub indication lights",
            src: JasperLight
        },
        {
            title: "Jude designing the personal device case",
            src: PersonalDeviceSketch
        },
        {
            title: "Working together on the conference room table",
            src: SharedTable
        },
        {
            title: "Taine dealing with the cringe",
            src: TaineL
        },
        {
            title: "Whiteboarding mesh network ideas",
            src: WhiteBoard
        },
        {
            title: "Weather station / Relay box",
            src: WeatherStationBox
        },
        {
            title: "Jude demoing the personal device",
            src: JudeDemoPersonalDevice
        },
        {
            title: "Signal Quo's logo being applied to a shirt",
            src: SignalQuoShirtLogo
        }
    ]
    console.log("asd",SignalQuoShirtLogo);
</script>
<div class="body-text">
<h1>About Signal Quo</h1>
<p>
    Signal Quo is an prototype built by Somewhat Questionable. Our
    team claimed a first-place finish at <a
        href="https://ymcachch.org.nz/grapple/"
        target="_blank">Grapple23</a
    >, a week-long engineering competition, held in
    <a href="https://4c.nz/">YMCA’s 4C Centre</a> in Christchurch, New Zealand.
</p>

<p>
    The prompt was to build something that would improve life on Mars. During the week, we developed a secondary emergency communication network for a Mars colony. Consisting of a main hub, personal transmitters, and environmental monitoring devices, Signal Quo is designed to operate during even severe Martian dust storms. The system utilizes long range (LoRa) technology, and incorporates a novel HopTrace protocol that records each signal's path, enabling geolocation for nodes in distress.
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
        <span class="name">Jasper Miller-Waugh</span> and <span class="name">Taine Reader</span> were our Whangarei
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
</div>
<LightboxGallery  enableImageExpand customization={{lightboxFooterProps: {style: "display: none;"}}} arrowsConfig={{color: "#fff",character: 'hide', enableKeyboardControl: true}} >
    <!-- Layout-->
    <svelte:fragment slot="thumbnail">
        <div class="galleryLayout">
            {#each images as image, i}
                <GalleryThumbnail id={i} >
                    <img src={image.src[0]} alt={image.title} title={image.title} class="image-{i}"/>
                </GalleryThumbnail>
            {/each}
        </div>
    </svelte:fragment>
    {#each images as image, i}
        <GalleryImage id={i} title="ssss">
            <img src={image.src[1]} alt={image.title} title={image.title} class="image-{i}"/>
        </GalleryImage>
    {/each}
</LightboxGallery>

<!-- 
<Gallery on:click={openImage} hover={true} loading maxColumnWidth={600}>
</Gallery> -->
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
        margin-bottom: 3em;
        position: relative;
        z-index: -1;
    }

    .galleryLayout {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 1em;
        :global(.svelte-lightbox-thumbnail) {
            border-radius: 0.5em;
            overflow: hidden;
            cursor: pointer;
            transition: transform 0.5s ease;
            :global(img) {
                width: 100%;
                height: 100%;
                object-fit: cover;
                cursor: pointer;
            }
            
        }
        :global(.svelte-lightbox-thumbnail:hover) {
            transform: scale(1.02);
        }
    }

    :global(.previous-button), :global(.next-button) {
        cursor: pointer;
    }

    .qrl-Logo {
        width: 256px;
        display: block;
        margin: 1em auto;
        margin: 5rem auto;
    }
</style>
