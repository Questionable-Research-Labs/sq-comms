<script lang="ts">
	import Nodes from "$lib/components/Nodes.svelte";
	import LiveGraphs from "$lib/components/LiveGraphs.svelte";
    import dayjs from "dayjs";
    import Particles from "$lib/components/Particles.svelte";

	let pageState: "nodes" | "sensors" = "nodes";
</script>

<svelte:head>
	<title>Home</title>
	<meta name="description" content="Svelte demo app" />
</svelte:head>

<nav>
	<button on:click={()=>{pageState="nodes"}} class:selected={pageState=="nodes"}>Nodes</button>
	<button on:click={()=>{pageState="sensors"}} class:selected={pageState=="sensors"}>Sensors</button>
</nav>

<div class="switching-content">
	<Particles />
	<div class="switching-content-wrapper">
		{#if pageState=="nodes"}
			<Nodes />
		{:else if pageState=="sensors"}
			<LiveGraphs />
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
