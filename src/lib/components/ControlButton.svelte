<script lang="ts">
	import { createEventDispatcher } from 'svelte';
	import { onDestroy } from 'svelte';

	export let isDisabled: boolean = false;
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

	const holdingClass = 'rounded-[15px] bg-bb-green-2 w-full h-full text-white';
</script>

<button
	on:mousedown={handleMouseDown}
	on:mouseup={handleMouseUp}
	on:touchstart={handleTouchStart}
	on:touchend={handleTouchEnd}
	class={isDisabled
		? 'pointer-events-none select-none rounded-[15px] bg-bb-green-1 unselectable'
		: holding
			? holdingClass
			: 'rounded-[15px] bg-bb-green-1 text-bb-black select-none'}
	disabled={open}
>
	<div class="grid items-center justify-center p-4 pointer-events-none select-none">
		{#if !open}
			<div class="flex gap-2 flex-col items-center justify-center">
				<!-- Header -->
				<h5>{boxType}</h5>

				<!-- Image and lock -->
				<div class="relative flex justify-center items-center">
					<img src={'/mockups/mailbox.png'} alt="mailbox mockup" class="w-[40%] md:w-[50%]" />
					<div class="scale-in absolute flex justify-center items-center w-full h-full">
						<div
							class={isDisabled
								? 'bg-[#ABABAB]/[0.7] w-[40px] h-[40px] flex items-center justify-center rounded-full'
								: holding
									? 'loader bg-black w-[20%] aspect-square flex rounded-full text-white'
									: 'bg-black w-[20%] aspect-square flex rounded-full text-white'}
						>
							<div class="flex flex-col justify-center items-center">
								<span class:rotate={holding}>
									{#if holding}
										<video
											autoplay
											controls={false}
											on:click={() => {
												return false;
											}}
											muted 
											playsinline
											class=" w-[60%] ml-[20%] aspect-square -rotate-45"
										>
											<source src="/icons/lock.mp4" type="video/mp4" />
											<track kind="captions" />
										</video>
									{:else}
										<img
											src={'/icons/lockStaticClosed.png'}
											alt="static lock"
											class="w-[58%] ml-[20%] mb-[1%]"
										/>
									{/if}
								</span>
							</div>
						</div>
					</div>
				</div>

				<!-- Locked Text -->
				<div
					class={holding ? 'flex flex-col -gap-0 text-white' : 'flex flex-col -gap-0 text-bb-black'}
				>
					{#if isDisabled}
						<p>Locked</p>
						<h6 class="mt-2 prevent-select">Wait for photo</h6>
					{:else}
						<p>Locked</p>
						{#if !holding}
							<h6 class="mt-2 prevent-select">Hold to open</h6>
						{:else}
							<h6 class="mt-2 prevent-select">Opening</h6>
						{/if}
					{/if}
				</div>
			</div>
		{:else}
			<div class="flex gap-2 flex-col items-center justify-center">
				<!-- Header -->
				<h5>{boxType}</h5>

				<!-- Image and lock -->
				<div class="relative flex justify-center items-center">
					<img src={'/mockups/mailbox.png'} alt="mailbox mockup" class="w-[40%] md:w-[50%]" />
					<div class="absolute flex justify-center items-center w-full h-full">
						<div class="scale-out">
							<div
								class={holding
									? 'loader bg-black w-[40px] h-[40px] flex items-center justify-center rounded-full text-white'
									: 'bg-black w-[40px] h-[40px] flex items-center justify-center rounded-full text-white'}
							>
								<div
									class="flex flex-col bg-black w-[25px] h-[25px] rounded-full justify-center items-center"
								>
									<span>
										<img src={'/icons/lockStaticOpened.gif'} alt="static lock" class="w-[24px]" />
									</span>
								</div>
							</div>
						</div>
					</div>
				</div>

				<!-- Locked Text -->
				<div class="flex flex-col -gap-1 text-bb-dark-green">
					<p class="prevent-select">Unlocked</p>
					<h6 class="mt-2 text-bb-dark-green prevent-select">Wait to lock</h6>
				</div>
			</div>
		{/if}
	</div>
</button>

<style>
	.prevent-select {
		-webkit-user-select: none; /* Safari */
		-ms-user-select: none; /* IE 10 and IE 11 */
		user-select: none; /* Standard syntax */
	}

	.unselectable {
		-moz-user-select: -moz-none;
		-khtml-user-select: none;
		-webkit-user-select: none;

		/*
			Introduced in IE 10.
			See http://ie.microsoft.com/testdrive/HTML5/msUserSelect/
		*/
		-ms-user-select: none;
		user-select: none;
	}

	.scale-out {
		animation-name: scale-out;
		animation-duration: 0.5s;
		animation-fill-mode: forwards;
		animation-timing-function: cubic-bezier(0.42, 0, 0.58, 1);
	}

	@keyframes scale-out {
		0% {
			transform: scale(1);
		}
		10% {
			transform: scale(1.2);
		}
		100% {
			transform: scale(0);
		}
	}

	.scale-in {
		animation-name: scale-in;
		animation-duration: 0.5s;
		animation-fill-mode: forwards;
		animation-timing-function: cubic-bezier(0.42, 0, 0.58, 1);
	}

	@keyframes scale-in {
		0% {
			transform: scale(0);
		}
		90% {
			transform: scale(1.2);
		}
		100% {
			transform: scale(1);
		}
	}

	.loader {
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
		border: 5px solid #1f2a27;
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
