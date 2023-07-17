<script lang="ts">
    import {config} from "./particlesConfig";
    import { onMount } from "svelte";
    //import { loadFull } from "tsparticles"; // if you are going to use `loadFull`, install the "tsparticles" package too.
    import { loadSlim } from "tsparticles-slim"; // if you are going to use `loadSlim`, install the "tsparticles-slim" package too.
    
    import type { Engine } from "tsparticles-engine";
    import type ParticlesComponent from "svelte-particles";
    import type { ParticlesEvents } from "svelte-particles";

    let ParticlesComponentDynamic: typeof ParticlesComponent;

    onMount(async () => {
        const module = await import("svelte-particles");

        ParticlesComponentDynamic = module.default;
    });

    async function particlesInit(engine: Engine) {
        // you can use main to customize the tsParticles instance adding presets or custom shapes
        // this loads the tsparticles package bundle, it's the easiest method for getting everything ready
        // starting from v2 you can add only the features you need reducing the bundle size
        //await loadFull(main);
        await loadSlim(engine);
    };

</script>

<svelte:component
    this="{ParticlesComponentDynamic}"
    id="tsparticles"
    class="tsparticles"
    options={config}
    particlesInit="{particlesInit}"
/>