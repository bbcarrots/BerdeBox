<script lang="ts">
	import { initializeFirebaseUI } from '$lib/firebase/auth';
	import { onMount } from 'svelte';
	import { login } from '$lib/firebase/auth';

	onMount(() => {
		//initialize the authentiation
		// initializeFirebaseUI();
	});
</script>

<section class="grid grid-cols-1 md:grid-cols-2 items-center w-full h-svh justify-center">
	<div
		class="flex flex-col md:justify-normal gap-8 justify-between md:max-h-[500px] xl:px-[20%] lg:px-[10%] px-[5%] absolute md:relative md:h-auto h-full p-8"
	>
		<div class="grid gap-4">
			<h1>Welcome to BerdeBox!</h1>
			<p>
				Receive packages safely and securely anywhere, anytime with the smart mailbox you can
				customize to fit your every need.
			</p>
		</div>
		<!-- <div id="firebaseui-auth-container"> -->
		<button
			class="flex gap-4 bg-bb-black text-white p-4 rounded-[15px] items-center justify-center"
			on:click={login}
		>
			<img class="object-cover w-[20px]" src="/logos/google-logo.svg" alt="google logo" /> Continue with
			Google
		</button>
		<!-- </div> -->
	</div>
	<div class="w-full h-svh">
		<img class="object-cover w-full h-full" src="/mockups/berdebox.png" alt="hero" />
	</div>
</section>
