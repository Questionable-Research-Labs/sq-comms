<script lang="ts">
    import { getContext } from "svelte";
    import { contextKey } from "@beyonk/svelte-mapbox";
    import { createGeoJSONCircle } from "$lib/helper";

    export let lat: number = 0;
    export let lng: number = 0;
    export let radius_meters: number = 0;

    const { getMap, getMapbox } = getContext(contextKey) as any;
    
    const map = getMap();
    const mapbox = getMapbox();

    const randomID = Math.random().toString(36).substring(2, 15) + Math.random().toString(36).substring(2, 15);

    map.addSource(`mapCircle_${randomID}`, createGeoJSONCircle([lng, lat], radius_meters/1000));

  map.addLayer({
    id: `mapCircle_${randomID}`,
    type: "fill",
    source: `mapCircle_${randomID}`,
    layout: {},
    paint: {
        "fill-color": "#13944f",
        "fill-opacity": 0.6,
        "fill-outline-color": "#fff",
    }
  });

</script>
