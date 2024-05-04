<script lan="ts">
	import {
		inputOutputRef,
		inputRef,
		outputRef,
		updateButton,
		updateLED,
		updateMainLock
	} from '$lib/firebase/database';
	import { InputOutput, Output, Input } from '$lib/stores/IO';
	import { onValue } from '@firebase/database';

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

	onValue(outputRef, (snapshot) => {
		const data = snapshot.val();
		$Output = data;
		console.log($Output);
	});

	onValue(inputOutputRef, (snapshot) => {
		const data = snapshot.val();
		$InputOutput = data;
		console.log($InputOutput);
	});

	onValue(inputRef, (snapshot) => {
		const data = snapshot.val();
		$Input = data;
		console.log($Input);
	});
</script>

<button on:click={handleOutput}>Button 1!</button>
<button on:click={handleInputOutput}>Button 2!</button>
<button on:click={handleInput}>Button 3!</button>
