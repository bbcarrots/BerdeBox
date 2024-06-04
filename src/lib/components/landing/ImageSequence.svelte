<script lang="ts">
	import { onMount } from 'svelte';

	export let isEnd: boolean = false;
	export let initialY: number;
	export let scrollY: number;
	export let duration: number;
	export let images: string[];
	export let frameCount: number;
	

	let section: HTMLElement;
	export let canvas: HTMLCanvasElement;
	let imageSources: HTMLImageElement[] = [];

	function loadImages() {
		imageSources = images.map((src) => {
			const img = new Image();
			img.src = src;
			return img;
		});
	}

	function sizeCanvas() {
		canvas.width = window.outerWidth;
		canvas.height = window.outerHeight;
	}

	function clearCanvas() {
		const context = canvas.getContext('2d');
		context?.clearRect(0, 0, canvas.width, canvas.height);
	}

	function drawImage(frame: number) {
		const context = canvas.getContext('2d');
		const image = imageSources[frame];
		const hRatio = canvas.width / image.width;
		const vRatio = canvas.height / image.height;
		const ratio = Math.min(hRatio, vRatio);
		const centerShiftX = (canvas.width - image.width * ratio) / 2;
		const centerShiftY = (canvas.height - image.height * ratio) / 2;

		clearCanvas();

		requestAnimationFrame(() => {
			context?.drawImage(
				image,
				0,
				0,
				image.width,
				image.height,
				centerShiftX,
				centerShiftY,
				image.width * ratio,
				image.height * ratio
			);
		});
	}

	onMount(() => {
		loadImages();
		sizeCanvas();
		drawImage(0);
	});

	let netScrollY: number;
	$: {
		netScrollY = scrollY - initialY;
		if (
			netScrollY >= section?.offsetTop &&
			netScrollY <= section?.offsetTop + section?.offsetHeight
		) {
			const scrollTop = netScrollY - section.offsetTop;
			const contentHeight = canvas.clientHeight;
			const progress = scrollTop / contentHeight;
			const frame = Math.max(0, Math.min(images.length - 1, Math.floor(progress * images.length)));
			drawImage(frame);

			console.log("HERE", scrollY, duration)
			if (scrollY >= duration/2){
				isEnd = true;
			}  else{
				isEnd = false;
			}
		} else {
			isEnd = true;

		}
	}
</script>

<section bind:this={section} style="--duration: {duration}px;">
	<canvas class="h-auto" bind:this={canvas} />
</section>

<style>
	section {
		width: 100%;
		height: var(--duration);
		position: relative;
		z-index: 10;
	}

	section > canvas {
		width: 100%;

		inset: 0;
		position: sticky;
		/* object-fit: contain; */
		pointer-events: none;
	}
</style>
