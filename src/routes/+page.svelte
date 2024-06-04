<script lang="ts">
	import Nav from '$lib/components/landing/Nav.svelte';
	import Footer from '$lib/components/landing/Footer.svelte';
	import ImageSequence from '$lib/components/landing/ImageSequence.svelte';
	import TextSequence from '$lib/components/landing/TextSequence_old.svelte';
	import { cubicOut } from 'svelte/easing';
	import FeatureCard from '$lib/components/landing/FeatureCard.svelte';

	let scrollY: number;

	const framesIntro = 61;
	const framesModules = 25;

	// Animation
	// -----------------------------------------------------------------
	let intro = Array.from({ length: framesIntro }, (_, i) => i).map((i) => {	
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/intro/${paddedIndex}.png`;
	});

	let cashbox = Array.from({ length: framesModules }, (_, i) => i).map((i) => {
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/modules/cashbox/${paddedIndex}.png`;
	});

	let uvlight = Array.from({ length: framesModules }, (_, i) => i).map((i) => {
		const paddedIndex = i.toString().padStart(4, '0');
		return `/landing/modules/uvbox/${paddedIndex}.png`;
	});

	let solarPanel = Array.from({ length: framesModules }, (_, i) => i).map((i) => {
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
</script>

<svelte:window bind:scrollY />

<Nav {scrollY}/>
<body>
	<div class="bg-white flex flex-col items-center">
		<!-- INTRO -->
		<div class="relative w-full bg-white">
			<div class="">
				<section class="full-screen" style="opacity: {calculateOpacity(scrollY, 250)}" />
				<ImageSequence initialY={0} {scrollY} duration={220} images={intro} />
			</div>
			<TextSequence {scrollY} textFrames={text} />
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
						subtitle={'Get real-time notifications when the doorbell is pressed.'}
					/>
					<FeatureCard
						src={'/landing/camera-stills/Camera.png'}
						title={'Camera'}
						subtitle={'Keep an eye on your mail 24/7.'}
					/>
					<FeatureCard
						src={'/landing/camera-stills/Speaker.png'}
						title={'Buzzer'}
						subtitle={'So deliverymen can easily locate your mailbox.'}
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
				<h3 class="w-[60%] whitespace-normal text-center">Add-as-you-need modules</h3>
			</div>
		</div>

		<!-- CASHBOX MODULE -->
		<div class="relative w-full h-[70vh]">
			<div class="absolute max-w-[60%]">
				<ImageSequence initialY={3600} {scrollY} duration={60} images={cashbox} />
			</div>
			<div class="absolute w-full h-[50vh] items-center grid grid-cols-2 z-50 text-bb-black">
				<div />
				<div class="flex flex-col gap-4 relative p-[20%]">
					<h3>Cashbox</h3>
					<p>Release money remotely with full control. Attach multiple cashboxes to transact with multiple deliverymen.</p>
				</div>
			</div>
		</div>

		<!-- UV LIGHT MODULE -->
		<div class="relative w-full h-[70vh]">
			<div class="absolute max-w-[60%]">
				<ImageSequence initialY={4400} {scrollY} duration={60} images={uvlight} />
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
		<div class="relative w-full h-[70vh]">
			<div class="absolute max-w-[60%]">
				<ImageSequence initialY={5300} {scrollY} duration={60} images={solarPanel} />
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
							Take control of your package reception. Pre-order your BerdeBox today and get a free cashbox module.
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
</style>
