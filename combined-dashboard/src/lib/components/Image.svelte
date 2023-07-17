<script lang="ts">

   interface Picture {
    sources: {
      avif: {
        src: string;
        w: number;
      }[];
      webp: {
        src: string;
        w: number;
      }[];
    };
    img: {
      src: string;
      w: number;
      h: number;
    };
   }

  /** REQUIRED */

  export let src: Picture;

  export let alt: string = "";

  /** OPTIONAL */

  export let draggable: boolean = false;

  /** @type {'async' | 'sync' | 'auto'} */
  export let decoding: 'async' | 'sync' | 'auto' = "async";

  /** @type {'lazy' | 'eager'} */
  export let loading: 'lazy' | 'eager' = "lazy";

  // export let hAlign: "left" | "right" | "center" = "center";
  // export let vAlign: "top" | "bottom" | "center" = "center";

  let classes = "";
  export { classes as class };
</script>
<!-- <div  class="h-{hAlign} v-{vAlign} image-container"> -->
  <picture>
  {#each Object.entries(src.sources) as [format, images]}
    <source
      srcset={images.map((i) => `${i.src} ${i.w}w`).join(", ")}
      type={"image/" + format}
    />
  {/each}

  <img
    src={src.img.src}
    {alt}
    class={classes}
    {loading}
    {decoding}
    {draggable}
    width={src.img.w}
    height={src.img.h}
  />
</picture>

<!-- </div> -->
<!-- <style lang="scss">
  .image-container {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100%;
    height: 100%;
    &.h-left {
      justify-content: flex-start;
    }
    &.h-right {
      justify-content: flex-end;
    }
    &.v-top {
      align-items: flex-start;
    }
    &.v-bottom {
      align-items: flex-end;
    }
    &.v-center {
      align-items: center;
    }
    &.h-center {
      justify-content: center;
    }
  }
</style> -->