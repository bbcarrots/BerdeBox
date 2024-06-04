<script lang="ts">
	import Nav from '$lib/components/landing/Nav.svelte';
	import Footer from '$lib/components/landing/Footer.svelte';
	import ImageSequence from '$lib/components/landing/ImageSequence.svelte';
	import { cubicOut } from 'svelte/easing';
	import FeatureCard from '$lib/components/landing/FeatureCard.svelte';
	import { onMount } from 'svelte';

	let isEnd: boolean;
	let scrollY: number;

	let cashboxElement: HTMLElement;
	let uvboxElement: HTMLElement;
	let solarElement: HTMLElement;

	let cashboxInitialY: number = 0;
	let uvboxInitialY: number = 0;
	let solarInitialY: number = 0;

	const framesIntro = 30;
	const framesModules = 12;

	// Animation
	// -----------------------------------------------------------------
	let intro = Array.from({ length: framesIntro }, (_, i) => i).map((i) => {
		i = i * 2 + 1;
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/intro/${paddedIndex}.png`;
	});

	let cashbox = Array.from({ length: framesModules }, (_, i) => i).map((i) => {
		i = i * 2 + 1;
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/modules/cashbox/${paddedIndex}.png`;
	});

	let uvlight = Array.from({ length: framesModules }, (_, i) => i).map((i) => {
		i = i * 2 + 1;
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/modules/uvbox/${paddedIndex}.png`;
	});

	let solarPanel = Array.from({ length: framesModules }, (_, i) => i).map((i) => {
		i = i * 2 + 1;
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/modules/solar-panel/${paddedIndex}.png`;
	});

	// Text
	// -----------------------------------------------------------------
	let text = Array.from({ length: framesIntro * 2 }, (_, i) => i).map((i) => {
		return {
			frame: Number(i),
			text: `frame ${i}`
		};
	});

	const calculateOpacity = (scrollY: number, fadeDistance: number) => {
		const t = Math.min(1, Math.max(scrollY / fadeDistance, 0.2));
		return cubicOut(1 - t);
	};

	// INTRO
	let introElement: HTMLElement;
	let introText: any;
	let canvasElement: HTMLCanvasElement;
	let introCanvasHeight: any;
	let cashboxCanvasHeight: any;
	let uvboxCanvasHeight: any;
	let solarCanvasHeight: any;

	let cashboxCanvasElement: HTMLCanvasElement;
	let uvboxCanvasElement: HTMLCanvasElement;
	let solarCanvasElement: HTMLCanvasElement;

	onMount(() => {
		introText = introElement.getBoundingClientRect();

		console.log(introText.top, introText.right, introText.bottom, introText.left);
	})

	$: {
		console.log(typeof window, introElement)
		if (typeof window !== 'undefined' && introElement && scrollY) {
			introText = introElement.getBoundingClientRect();
			introCanvasHeight = canvasElement.getBoundingClientRect().height;
			console.log("duration", introCanvasHeight)

			console.log(introText.top, introText.right, introText.bottom, introText.left);
			// Update initialY values for modules
			cashboxCanvasHeight = cashboxElement.getBoundingClientRect().height - 30;
			uvboxCanvasHeight = uvboxElement.getBoundingClientRect().height - 30;
			solarCanvasHeight = cashboxElement.getBoundingClientRect().height - 30;
			cashboxInitialY = cashboxElement.getBoundingClientRect().top + scrollY - cashboxCanvasHeight;
			uvboxInitialY = uvboxElement.getBoundingClientRect().top + scrollY - uvboxCanvasHeight;
			solarInitialY = solarElement.getBoundingClientRect().top + scrollY - solarCanvasHeight;
		}
	}
	
</script>

<svelte:window bind:scrollY />

<Nav {scrollY}/>

<body>
	<div class="relative bg-white flex flex-col items-center w-full">
		<!-- INTRO -->
		<div class="relative w-full bg-white">
			<div bind:this={introElement} 
				class={ isEnd ? "z-50 w-full" : "fixed top-[30%] z-50 w-full"}
				style={ isEnd ? `position: absolute; top: ${introText.top + scrollY}px; left: ${introText.left}px; right: ${introText.right}px; bottom: ${introText.bottom + scrollY}px; z-index: 50; width: 100%;` : ''}
			>
				<div class="grid text-bb-black gap-[27px] text-content animate-fade-in text-center">
					<h1 class="text-[121px] animate-gradient">BerdeBox</h1>
					<h5 class="text-[25.25px] pt-[10px] text-bb-dark-green/[.8]">Receive packages securely from anywhere, anytime</h5>
					<div class="flex gap-4">
					</div>
				</div>
			</div>
			<div class="relative z-40">
				<section class="full-screen" style="opacity: {calculateOpacity(scrollY, 250)}" />
				<ImageSequence bind:canvas={canvasElement} frameCount={framesIntro} bind:isEnd initialY={0} {scrollY} duration={2*introCanvasHeight} images={intro} />
			</div>
		</div>

		<!-- FEATURES -->
		<!-- header -->
		<div class="flex flex-row md:m-40 m-10 bg-white items-center justify-center text-bb-black">
			<div class="flex flex-col items-center gap-2">
				<h5>Features</h5>
				<h3 class="w-[60%] text-center">Your personal delivery assistant</h3>
			</div>
		</div>

		<!-- content -->
		<div id="features" class="relative container mx-auto py-10 z-40">
			<div class="grid gap-8 mb-10">
				<!-- FEATURE CARDS -->
				<div class="grid grid-cols-1 md:grid-cols-3 gap-8">
					<!-- each feature card -->
					<FeatureCard
						src={'/landing/camera-stills/Button.png'}
						title={'Doorbell'}
						subtitle={'Get real-time notifications on the mobile app when the doorbell is pressed'}
					/>
					<FeatureCard
						src={'/landing/camera-stills/Camera.png'}
						title={'Camera'}
						subtitle={'Keep an eye on your mail 24/7 with a 2 megapixel OV2640 camera'}
					/>
					<FeatureCard
						src={'/landing/camera-stills/Speaker.png'}
						title={'Buzzer'}
						subtitle={'Designed so deliverymen can easily locate your BerdeBox'}
					/>
				</div>

				<!-- MAIN OVERVIEW CARD -->
				<div
					class="hero h-[100vh] lg:h-[45rem] bg-[#F8F8F8] rounded-[25px] flex items-center justify-start p-8"
					style="background-image: url('/landing/features-image.png'); background-size: cover; background-position: center;"
				>
					<div class="max-w-lg text-left text-bb-black flex flex-col space-y-4 ml-auto pl-16">
						<h4>Your packages, safe and secure</h4>
						<p class="text-xl">Auto-locking system</p>
						<p>Fool-proof measures to ensure the BerdeBox is always locked after a transaction</p>
						<!-- <button class="btn btn-primary w-full">Get Started</button> -->
					</div>
				</div>
			</div>
		</div>

		<!-- MODULES -->
		<div id="modules" class="flex flex-row m-40 bg-white items-center justify-center text-bb-black">
			<div class="flex flex-col items-center gap-2">
				<h5>Modules</h5>
				<h3 class="w-[60%] whitespace-normal text-center">Plug-and-play modules</h3>
			</div>
		</div>

		<!-- CASHBOX MODULE -->
		<div class="relative w-full h-[70vh]" bind:this={cashboxElement}>
			<div class="absolute max-w-[60%]">
				<ImageSequence bind:canvas={cashboxCanvasElement} frameCount={framesModules} initialY={cashboxInitialY} {scrollY} duration={cashboxCanvasHeight} images={cashbox} />
			</div>
			<div class="absolute w-full h-[50vh] items-center grid grid-cols-2 z-50 text-bb-black">
				<div />
				<div class="flex flex-col gap-4 relative p-[20%]">
					<h3>Cashbox</h3>
					<p>Release money remotely with full control - attach multiple cashboxes to transact with multiple deliverymen</p>
				</div>
			</div>
		</div>

		<!-- UV LIGHT MODULE -->
		<div class="relative w-full h-[70vh]" bind:this={uvboxElement}>
			<div class="absolute max-w-[60%]">
				<ImageSequence bind:canvas={uvboxCanvasElement} frameCount={framesModules} initialY={uvboxInitialY} {scrollY} duration={uvboxCanvasHeight} images={uvlight} />
			</div>
			<div class="absolute w-full h-[50vh] items-center grid grid-cols-2 z-50 text-bb-black">
				<div />
				<div class="flex flex-col gap-4 relative p-[20%]">
					<h3>UV Lightbox</h3>
					<p>Automatically disinfects the contents of your BerdeBox after a successful transaction.</p>
				</div>
			</div>
		</div>

		<!-- Solar Panel -->
		<div class="relative w-full h-[70vh]" bind:this={solarElement}>
			<div class="absolute max-w-[60%]">
				<ImageSequence bind:canvas={solarCanvasElement} frameCount={framesModules} initialY={solarInitialY} {scrollY} duration={solarCanvasHeight} images={solarPanel} />
			</div>
			<div class="absolute w-full h-[50vh] items-center grid grid-cols-2 z-50 text-bb-black">
				<div />
				<div class="flex flex-col gap-4 relative p-[20%]">
					<h3>Solar Panel</h3>
					<p>Power your BerdeBox with the sun - no more replacing batteries.</p>
				</div>
			</div>
		</div>

		<!-- CALL TO ACTION -->
		<div class="relative grid grid-cols-2 w-[90%] items-center justify-center gap-4 bg-[#F8F8F8] mt-40 rounded-[25px] overflow-hidden z-50">
			<!-- Text -->
			<div>
				<div class="flex flex-col p-20 text-bb-black gap-16 w-[90%] ml-10">
					<div class="grid items-center justify-center gap-4">
						<h4>“Kuya, iwan mo nalang sa BerdeBox.”</h4>
						<p class="">
							Take control of your online deliveries. Pre-order your BerdeBox today and get a free cashbox module.
						</p>
					</div>
					
					<!-- Button -->
					<div class="flex gap-4">
						<button class="w-[160px] border border-bb-light-green bg-gradient-to-tr from-bb-light-green p-4 hover:text-white hover:bg-bb-green shadow-[0px 14px 50px 0px #557760] flex flex-col items-center rounded-[15px] duration-300 ease-in-out">
							Pre-order Now
						</button>
						<a href="/app" class="button-link">
							<button class="w-[160px] border border-bb-light-green p-4 hover:text-white hover:bg-bb-green shadow-[0px 14px 50px 0px #557760] flex flex-col items-center rounded-[15px] duration-300 ease-in-out">
								Try BerdeBox
							</button>
						</a>

					</div>

				</div>


			</div>


			<!-- Image -->
			<img src="/mockups/7.png" alt="berdebox still"/>
		</div>

	</div>
</body>
<Footer/>

<style>
	@keyframes fadeOut {
		from {
			opacity: 1;
		}
		to {
			opacity: 0;
		}
	}

	.full-screen {
		position: fixed;
		top: 0;
		left: 0;
		width: 100vw;
		height: 100vh;
		background: radial-gradient(circle at 10% 20%, rgb(0, 0, 0) 0%, rgb(20, 20, 20) 99%);
		transition: opacity 0.1s;
	}

	@keyframes grey-black-gradient {
            0% {
                background-position: 0% 50%;
            }
            100% {
                background-position: 100% 50%;
            }
        }

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
