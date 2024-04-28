<script lan='ts'>
	import { writeLed } from "$lib/firebase/database";

    let isOnLed = false;

    function handleClick() {
        isOnLed = !isOnLed
        writeLed(isOnLed)
    }
</script>

<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>
<button on:click={handleClick}>ClickMe!</button>