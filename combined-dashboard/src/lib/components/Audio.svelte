<script lang="ts">
    import Mute from "$lib/icons/Mute.svelte";
    import Sound from "$lib/icons/Sound.svelte";
    import { alertTable, type DistanceMeasurement } from "$lib/store";
    import { Howl, Howler } from "howler";
    
    let soundMuted = true;
    let soundEnabled = false;
    const sound = new Howl({
        src: ["alarm.mp3"],
        volume: 0.5,
    });

    let playSoundInterval: number | null = null;

    function soundLoop() {
        if (soundEnabled) {
            sound.play();
        }
    }

    function controlSound(alertTable: Map<string,DistanceMeasurement>) {
        let enable = alertTable.size > 0;
        if (enable && !playSoundInterval) {
            playSoundInterval = setInterval(soundLoop, 1056);
            soundLoop();
        } else if (!enable && playSoundInterval) {
            clearInterval(playSoundInterval);
            playSoundInterval = null;
            sound.stop();
        }
    }
    $: controlSound($alertTable);
    
</script>

<button
    title="Toggle Alert Sound"
    on:click={() => {
        soundEnabled = true;
        soundMuted = !soundMuted;
        if (soundMuted) {
            sound.mute(true);
        } else {
            sound.mute(false);
        }
    }}
>
    {#if soundMuted}
        <Mute />
    {:else}
        <Sound />
    {/if}
</button>

<style lang="scss">
    @use "sass:math";
    @use "../../variables.scss" as *;
    $button-size: 2rem;
    button {
        position: absolute;
        top: math.div($button-size, 2);
        right: math.div($button-size, 2);
        height: $button-size;
        width: $button-size;
        padding: 0;
        border: none;
        background: none;
        cursor: pointer;
        :global(svg) {
            height: 100%;
            fill: $color-text
        }
    }
</style>