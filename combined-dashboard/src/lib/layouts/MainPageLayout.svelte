<script lang="ts">
    import Particles from "$lib/components/Particles.svelte";
    import { page } from "$app/stores";

	let pageState: "nodes" | "sensors" | "about" = $page.url.pathname.split("/")[1].replace("/","") || "nodes";
    console.log($page.url.pathname)
</script>

<svelte:head>
	<title>Signal Quo</title>
	<meta name="description" content="Signal Quo is an prototype built by Somewhat Questionable during Grapple23. It's an emergency secondary communication network designed for a Mars colony." />
</svelte:head>

<nav>
	<a on:click={()=>{pageState = "nodes"}} href="/" class:selected={pageState=="nodes"}>Nodes</a>
	<a on:click={()=>{pageState = "sensors"}} href="/sensors" class:selected={pageState=="sensors"}>Sensors</a>
	<a on:click={()=>{pageState = "about"}} href="/about" class:selected={pageState=="about"}>About</a>
</nav>

<div class="switching-content">
	<Particles />
	<div class="switching-content-wrapper">
        <slot/>
	</div>
</div>


<style lang="scss">
	@use "../../variables.scss" as *;
	nav {
		display: flex;
		justify-content: center;
		gap: 1rem;
		padding: 1rem;
		// Fade from background to transparent
		background: linear-gradient(180deg, $color-bg-0 80%, rgba(0,0,0,0) 100%);
		box-sizing: border-box;
		a {
			display: block;
			color: $color-text;
			border: none;
			padding: 0.6rem;
			font-size: 1.2rem;
			text-transform: uppercase;
			font-weight: bold;
			background: none;
			cursor: pointer;
			border-bottom: 2px solid rgba(0,0,0,0);
            text-decoration: none;
			&:hover {
				color: $color-text-hover;
				border-bottom: 2px solid $color-text-hover;
			}
			&:focus {
				outline: none;
			}
			&.selected {
				color: $color-text-selected;
				border-bottom: 2px solid $color-text-selected;
			}
		}
	}

	.switching-content {
		display: flex;
		justify-content: center;
		padding: 1rem;
		position: relative;
		.switching-content-wrapper {
			width: 100%;
			max-width: 1200px;
		}
	}
</style>
