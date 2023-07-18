<script lang="ts">
	import Nodes from "$lib/components/Nodes.svelte";
	import LiveGraphs from "$lib/components/LiveGraphs.svelte";
    import dayjs from "dayjs";
    import Particles from "$lib/components/Particles.svelte";
	import About from "$lib/components/About.svelte";

	let pageState: "nodes" | "sensors" | "about" = "nodes";
</script>

<svelte:head>
	<title>Signal Quo</title>
	<meta name="description" content="Signal Quo is an prototype built by Somewhat Questionable during Grapple23. It's an emergency secondary communication network designed for a Mars colony." />
</svelte:head>

<nav>
	<button on:click={()=>{pageState="nodes"}} class:selected={pageState=="nodes"}>Nodes</button>
	<button on:click={()=>{pageState="sensors"}} class:selected={pageState=="sensors"}>Sensors</button>
	<button on:click={()=>{pageState="about"}} class:selected={pageState=="about"}>About</button>
</nav>

<div class="switching-content">
	<Particles />
	<div class="switching-content-wrapper">
		{#if pageState=="nodes"}
			<Nodes />
		{:else if pageState=="sensors"}
			<LiveGraphs />
		{:else if pageState==="about"}
			<About />
		{/if}
	</div>
</div>


<style lang="scss">
	@use "../variables.scss" as *;
	nav {
		display: flex;
		justify-content: center;
		gap: 1rem;
		padding: 1rem;
		// Fade from background to transparent
		background: linear-gradient(180deg, $color-bg-0 80%, rgba(0,0,0,0) 100%);
		box-sizing: border-box;
		button {
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
