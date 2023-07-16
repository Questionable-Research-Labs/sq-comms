<script>
  import "../app.scss";
  import Header from "$lib/components/Header.svelte";
  import { onMount } from "svelte";
  import { mqttConnect } from "$lib/mqtt";

  onMount(() => {
    mqttConnect();
  });
</script>

<div class="app">
  <Header />

  <main>
    <slot />
  </main>

  <footer>
    <p>
      visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to learn SvelteKit
    </p>
  </footer>
</div>

<style>
</style>
