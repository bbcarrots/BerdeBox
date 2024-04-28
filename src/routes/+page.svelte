<script lan="ts">
	import { updateButton, updateLED, updateMainLock } from '$lib/firebase/database';

	let isOnLed = false;
	let isOnMainLock = false;
	let isOnButton = false;

	function handleOutput() {
		isOnLed = !isOnLed;
		updateLED(isOnLed);
	}

	function handleInputOutput() {
		isOnMainLock = !isOnMainLock;
		updateMainLock(isOnMainLock);
	}

	function handleInput() {
		isOnButton = !isOnButton;
		updateButton(isOnButton);
	}
</script>

<h1>Welcome</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>
<button on:click={handleOutput}>Button 1!</button>
<button on:click={handleInputOutput}>Button 2!</button>
<button on:click={handleInput}>Button 3!</button>
