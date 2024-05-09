<script lang="ts">
    import { createEventDispatcher } from 'svelte';
    import { onDestroy } from 'svelte';

    export let boxType
  
    let timer: any;
    let holding: boolean = false; // Variable to track if button is being held down
    const dispatch = createEventDispatcher();
  
    function handleMouseDown() {
        holding = true; // Button is being held down
        timer = setTimeout(() => {
            dispatch('held');
        }, 1000);
    }
  
    function handleMouseUp() {
        clearTimeout(timer);
        holding = false; // Button is released
    }
  
    onDestroy(() => {
        // Ensure to clear the timer when the component is destroyed
        clearTimeout(timer);
    });
</script>

<button 
    on:mousedown={handleMouseDown} 
    on:mouseup={handleMouseUp} 
    class:holding={holding}
>
    <div class="grid border w-full h-[100px] items-center justify-center">
        <h5> {boxType} </h5>
        <p> Hold to open </p>
    </div>    
</button>

<style>
.holding {
    background-color: #ccc; 
}
</style>