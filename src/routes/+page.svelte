<script lang="ts">
	import { initializeFirebaseUI } from '$lib/firebase/auth';
	import { onMount } from 'svelte';
	import { getBerdeBoxes, getUsers, getBoxbyID, getUserbyID } from '$lib/firebase/firestore';
	import firebase from 'firebase/compat/app';
	onMount(() => {
		//initialize the authentiation
		initializeFirebaseUI();
		// getBerdeBoxes();
		// getUsers();
		// getBoxbyID('GyPMCDzKWXxlKS6HUdf0');
		// getBoxbyID('sdfg');

		// getUserbyID('yq62Vt8JholuEr2a1vWq');
		// getUserbyID('sdfgs');
	});
</script>

<section class="grid grid-cols-1 md:grid-cols-2 items-center w-full h-full justify-center">
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
		<div id="firebaseui-auth-container">
			<!-- <button class="flex gap-4 bg-bb-black text-white p-4 rounded-[15px] items-center justify-center">
				<img class="object-cover w-[20px]" src="/logos/google-logo.svg" alt="google logo"> Continue with Google
			</button> -->
		</div>
	</div>
	<div class="bg-red-100 w-full h-screen">
		<img class="object-cover w-full h-full" src="/mockups/berdebox.png" alt="hero" />
	</div>
</section>
