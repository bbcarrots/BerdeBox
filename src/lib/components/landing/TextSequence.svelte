<script lang="ts">
    import { onMount } from 'svelte';
  
    export let scrollY: number;
    export let duration: number;
    export let textFrames: { frame: number; text: string }[];
  
    let section: HTMLElement;
    let canvas: HTMLCanvasElement;
    let currentFrame = 0;
  
    function updateText(frame: number) {
      currentFrame = frame;
      drawText(textFrames[currentFrame].text);
    }
  
    function sizeCanvas() {
      canvas.width = window.innerWidth;
      canvas.height = window.innerHeight;
    }
  
    function clearCanvas() {
      const context = canvas.getContext('2d');
      context?.clearRect(0, 0, canvas.width, canvas.height);
    }
  
    function drawText(text: string) {
      const context = canvas.getContext('2d');
      clearCanvas();
  
      context!.font = '24px Arial';
      context!.fillStyle = 'black';
      context!.textAlign = 'center';
      context!.textBaseline = 'middle';
      context?.fillText(text, canvas.width / 2, canvas.height / 2);
    }
  
    onMount(() => {
      sizeCanvas();
      drawText(textFrames[0].text);
  
      window.addEventListener('resize', sizeCanvas);
  
      return () => {
        window.removeEventListener('resize', sizeCanvas);
      };
    });
  
    $: {
      if (scrollY >= section?.offsetTop && scrollY <= section?.offsetTop + section?.offsetHeight) {
        const scrollTop = scrollY - section.offsetTop;
        const contentHeight = section.offsetHeight;
        const progress = scrollTop / contentHeight;
        const frame = Math.max(0, Math.min(textFrames.length - 1, Math.floor(progress * textFrames.length)));
        updateText(frame);
      }
    }
  </script>
  
  <section
  bind:this={section}
  class="w-full fixed top-0 left-0 z-50"
  style="height: {duration || 100}vh;"
>
  <canvas
    bind:this={canvas}
    class="w-full h-screen absolute top-1/2 left-1/2 transform -translate-x-1/2 -translate-y-1/2 object-contain pointer-events-none"
  />
</section>