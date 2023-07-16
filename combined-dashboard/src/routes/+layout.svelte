<script>
  import "../app.scss";
  import Header from "$lib/components/Header.svelte";
  import { onMount } from "svelte";
  import { mqttConnect, mqttConnected } from "$lib/mqtt";
    import Audio from "$lib/components/Audio.svelte";

  onMount(() => {
    mqttConnect();
  });
</script>

<div class="app">
  <Header />
  <Audio/>

  <main>
    <slot />
  </main>
  <div class="connection-indicator">
    {#if $mqttConnected}
      <span class="connected">Connected</span>
    {:else}
      <span class="disconnected">Connecting...</span>
    {/if}
  </div>
</div>

<style lang="scss">
  .connection-indicator {
    position: fixed;
    bottom: 0;
    right: 0;
    padding: 0.5rem;
    background: #000000aa;
    color: white;
    font-size: 0.8rem;
    line-height: 1rem;
    font-weight: bold;
    span {
      margin-left: 0.5rem;
      &::before {
        content: "•";
        font-weight: bolder;
        font-size: 2rem;
        margin-right: 0.5rem;
        line-height: 1rem;
        vertical-align: middle;
      }
      &.connected::before {
          color: green;
        }
      &.disconnected::before {
        color: red;
      }
    }
  }
</style>