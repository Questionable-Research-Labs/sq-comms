<script>
  import "../app.scss";
  import "../modal.scss";

  import Header from "$lib/components/Header.svelte";
  import { onMount } from "svelte";
  import { mqttConnect, mqttConnected } from "$lib/mqtt";
  import Audio from "$lib/components/Audio.svelte";
  import Modal from "svelte-simple-modal";

  onMount(() => {
    mqttConnect();
  });
</script>

<svelte:head>
  <link rel="apple-touch-icon" sizes="152x152" href="/apple-touch-icon.png">
  <link rel="icon" type="image/png" sizes="32x32" href="/favicon-32x32.png">
  <link rel="icon" type="image/png" sizes="16x16" href="/favicon-16x16.png">
  <link rel="manifest" href="/site.webmanifest">
  <link rel="mask-icon" href="/safari-pinned-tab.svg" color="#9787ff">
  <meta name="msapplication-TileColor" content="#da532c">
  <meta name="theme-color" content="#2e2853">
</svelte:head>

<Modal
  unstyled={true}
  classBg="__modal-bg"
  classWindowWrap="__modal-window-wrap"
  classWindow="__modal-window"
  classContent="__modal-content fancy-scrollbar"
  classCloseButton="__modal-close-button"
>
  <div class="app">
    <Header />
    <Audio />

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
</Modal>

<style lang="scss">
  .connection-indicator {
    position: fixed;
    bottom: 0;
    right: 0;
    padding: 0.5rem;
    background: #00000077;
    color: white;
    font-size: 0.8rem;
    line-height: 1rem;
    font-weight: bold;
    border-radius: 1rem 0 0 1rem;
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
