<script lang="ts">
	import { createEventDispatcher } from 'svelte';
	import { onDestroy } from 'svelte';
	import { Icon, LockClosed } from 'svelte-hero-icons';

	export let boxType;
	export let open: boolean;

	let timer: any;
	let holding: boolean = false; // Variable to track if button is being held down
	const dispatch = createEventDispatcher();

	function handleMouseDown() {
		holding = true; // Button is being held down
		timer = setTimeout(() => {
			dispatch('held');
			holding = false; // Button is released
			open = true;
		}, 2000);
	}

	function handleMouseUp() {
		clearTimeout(timer);
		holding = false; // Button is released
	}

	function handleTouchStart() {
		holding = true; // Button is being held down
		timer = setTimeout(() => {
			dispatch('held');
			holding = false; // Button is released
			open = true;
		}, 2000);
	}

	function handleTouchEnd() {
		clearTimeout(timer);
		holding = false; // Button is released
	}

	onDestroy(() => {
		// Ensure to clear the timer when the component is destroyed
		clearTimeout(timer);
	});

	const holdingClass = 'rounded-[15px] bg-[#B4A5EC] w-full h-full';
</script>

<button
	on:mousedown={handleMouseDown}
	on:mouseup={handleMouseUp}
	on:touchstart={handleTouchStart}
	on:touchend={handleTouchEnd}
	class={holding === true ? holdingClass : 'rounded-[15px] bg-[#E7E2FA] w-full h-full'}
	disabled={open}
>
	<div class="grid items-center justify-center p-6 no-select">
		{#if !open}
			<div class="flex gap-2 flex-col items-center justify-center">
				<div>
					<h5>{boxType}</h5>
					<p>Locked</p>
				</div>

				<div
					class={holding === true
						? 'loader bg-bb-black w-[50px] h-[50px] flex items-center justify-center rounded-full text-white'
						: 'bg-bb-black w-[50px] h-[50px] flex items-center justify-center rounded-full text-white'}
				>
					<span class={holding === true ? '-rotate-45' : ''}>
						<Icon src={LockClosed} outline size="24" class="inline-block" />
					</span>
				</div>
				<h6 class="mt-2">Hold to open</h6>
			</div>
		{:else}
			<div class="flex gap-2 flex-col items-center justify-center text-[#8A8A8A]">
				<div>
					<h5>{boxType}</h5>
					<p>Unlocked</p>
				</div>

				<div
					class="bg-[#8A8A8A] w-[50px] h-[50px] flex items-center justify-center rounded-full text-white"
				>
					<span class={holding === true ? '-rotate-45' : ''}>
						<Icon src={LockClosed} outline size="24" class="inline-block" />
					</span>
				</div>
				<h6 class="mt-2">Reset lock</h6>
			</div>
		{/if}
	</div>
</button>

<style>
	/* HTML: <div class="loader"></div> */
	.loader {
		width: 50px;
		height: 50px;
		aspect-ratio: 1;
		border-radius: 50%;
		position: relative;
		transform: rotate(45deg);
	}

	.loader::before {
		content: '';
		position: absolute;
		inset: -5px;
		border-radius: 50%;
		border: 5px solid #514b82;
		animation: l18 2s infinite linear;
	}
	@keyframes l18 {
		0% {
			clip-path: polygon(50% 50%, 0 0, 0 0, 0 0, 0 0, 0 0);
		}
		25% {
			clip-path: polygon(50% 50%, 0 0, 100% 0, 100% 0, 100% 0, 100% 0);
		}
		50% {
			clip-path: polygon(50% 50%, 0 0, 100% 0, 100% 100%, 100% 100%, 100% 100%);
		}
		75% {
			clip-path: polygon(50% 50%, 0 0, 100% 0, 100% 100%, 0 100%, 0 100%);
		}
		100% {
			clip-path: polygon(50% 50%, 0 0, 100% 0, 100% 100%, 0 100%, 0 0);
		}
	}

	.no-select {
		user-select: none;
		-webkit-user-select: none;
		-moz-user-select: none;
		-ms-user-select: none;
	}
</style>
