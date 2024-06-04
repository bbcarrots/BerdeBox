<script lang="ts">
	import { onMount } from 'svelte';

	export let scrollY: number;
	export let textFrames: { frame: number; text: string }[];

	let section: HTMLElement;
	let textContainer: HTMLElement;
	let currentFrame = 0;

	function updateText(frame: number) {
		currentFrame = frame;
	}

	$: {
		if (scrollY >= section?.offsetTop && scrollY <= section?.offsetTop + section?.offsetHeight) {
			const scrollTop = scrollY - section.offsetTop;
			const contentHeight = section.offsetHeight;
			const progress = scrollTop / contentHeight;
			const frame = Math.max(
				0,
				Math.min(textFrames.length - 1, Math.floor(progress * textFrames.length))
			);
			updateText(frame);
		}
	}
</script>

<section
	bind:this={section}
	class="w-full h-full top-0 left-0 z-40 flex items-start justify-start fixed"
>
	<div
		class="text-container text-center absolute top-1/2 left-1/2 transform -translate-x-1/2 -translate-y-1/2"
		bind:this={textContainer}
	>
		{#each textFrames as textFrame, i}
			{#if i === currentFrame}
				<!-- <div class="text-frame text-2xl text-black opacity-0 animate-fade-in">{textFrame.text}</div> -->
			{/if}
		{/each}
	</div>

	<div
		class="flex text-container absolute top-1/2 left-0 transform -translate-y-1/2 w-full items-center justify-center"
		bind:this={textContainer}
	>
		{#if currentFrame >= 0 && currentFrame < 115}
			<div class="hero-content flex items-start justify-center w-full h-full relative z-10">
				<div class="grid text-bb-black gap-[27px] text-content animate-fade-in text-center">
					<h1 class="text-[121px] animate-gradient">BerdeBox</h1>
					<h5 class="text-[25.25px] pt-[10px] text-bb-dark-green/[.8]">Receive packages securely from anywhere, anytime</h5>
					<div class="flex gap-4">
					</div>
				</div>
			</div>
		{/if}
	</div>

</section>

<style>
	@keyframes fadeIn {
		0% {
			opacity: 0;
		}
		100% {
			opacity: 1;
		}
	}

	.animate-fade-in {
		animation: fadeIn 0.5s ease-in-out forwards;
	}

	@keyframes fadeOut {
		0% {
			opacity: 1;
		}
		100% {
			opacity: 0;
		}
	}

	.animate-fade-out {
		animation: fadeOut 0.5s ease-in-out forwards;
	}

	@keyframes gradient-move {
        0% {
            background-position: 0% 50%;
        }
        100% {
            background-position: 100% 50%;
        }
    }

    .animate-gradient {
        background: linear-gradient(270deg, #557760, #304336, #557760, #304336);
        background-size: 200% 200%;
        animation: gradient-move 12s ease infinite;
        -webkit-background-clip: text;
        -webkit-text-fill-color: transparent;
		opacity: 0.8;
	}
</style>
