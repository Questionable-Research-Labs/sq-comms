<script lang="ts">
  import { getContext, onDestroy, onMount } from "svelte";
  import { contextKey } from "@beyonk/svelte-mapbox";
  import { createGeoJSONCircle } from "$lib/helper";
  import type mapboxgl from "mapbox-gl";

  export let lat: number;
  export let lng: number;
  export let radius_meters: number;
  export let chipID: string;

  const { getMap, getMapbox } = getContext(contextKey) as any;

  const map: mapboxgl.Map = getMap();
  const mapbox = getMapbox();

  function updateSource(lng: number, lat: number, radius_meters: number) {
    if (!map) return;

    let newGen = createGeoJSONCircle([lng, lat], radius_meters / 1000);
    let source = map.getSource(`mapCircle_${chipID}`);

    if (source && source.type == "geojson") {
        source.setData(newGen.data as any);
    } else {
      map.addSource(`mapCircle_${chipID}`, newGen);
    }

    let layer = map.getLayer(`mapCircle_${chipID}_fill`);

    if (!layer) {
      map.addLayer({
        id: `mapCircle_${chipID}_fill`,
        type: "fill",
        source: `mapCircle_${chipID}`,
        layout: {},
        paint: {
          "fill-color": "#13944f",
          "fill-opacity": 0.6,
        },
      });
      map.addLayer({
        id: `mapCircle_${chipID}_stroke`,
        type: "line",
        source: `mapCircle_${chipID}`,
        layout: {},
        paint: {
          'line-color': 'rgba(255, 255, 255, 1)',
          'line-width': 2
        }
      });  
    }
  }
  $: updateSource(lng, lat, radius_meters);

  onDestroy(()=>{
    if (map) {
      map.removeLayer(`mapCircle_${chipID}_fill`);
      map.removeLayer(`mapCircle_${chipID}_stroke`);
      map.removeSource(`mapCircle_${chipID}`);
    }
  })
</script>
