<script lang="ts">
	import { onMount } from 'svelte';
	import '../app.css';
	import { auth } from '$lib/firebase/auth';
	import { onAuthStateChanged } from 'firebase/auth';
	import { UserStore, notifsPermitted } from '$lib/stores/User';
	import { getUserbyID, addUser, getBoxByRef } from '$lib/firebase/firestore';
	import { goto } from '$app/navigation';
	import { updateBoxesStore } from '$lib/utils/storeFunctions';
	import Loader from '$lib/components/Loader.svelte';
	import { loading } from '$lib/stores/Page';
	import { page } from '$app/stores';

	if ($page.url.pathname == "/") {
        loading.set(true)
    }

	onMount(() => {

		// HANDLES AUTHENTICATION
		onAuthStateChanged(auth, async (user) => {
			
			// If authentication succeeds
			if (user) {
				let validUser = await getUserbyID(user.uid); 

				// If the user exists, update the user store
				// then update the boxes store based on user inforation
				// then redirect to /boxes
				if (validUser !== null) {
					
					UserStore.set({
						uid: user.uid,
						notifToken: validUser?.notifToken,
						boxes: validUser?.berdeboxes,
						notifsPermitted: validUser?.notifsPermitted
					});

					updateBoxesStore($UserStore.boxes)
					goto('/boxes');
				}

				// If the user doesn't exist yet
				// add a new user document to firestore
				// then retrieve the user and update the user store
				// then reroute to /boxes
				else {
					addUser(user.uid);

					let validUser = await getUserbyID(user.uid); // Await the promise

					UserStore.set({
						uid: user.uid,
						notifToken: validUser?.notifToken,
						boxes: validUser?.berdeboxes,
						notifsPermitted: validUser?.notifsPermitted
					});

					goto('/boxes');
				}
			} 
			
			// If authentication fails
			else {
				loading.set(false)
				// Handle the case where there's no user
				console.log('No user is signed in');
			}
		});
	});
</script>

{#if $loading == false}
<body class="h-svh bg-[#EEF2F5]">
	<slot />
</body>
{:else}
	<Loader></Loader>
{/if}

<style>
	@import url('https://fonts.googleapis.com/css2?family=Familjen+Grotesk:ital,wght@0,400..700;1,400..700&display=swap');

	:global(body) {
		font-family: 'Familjen Grotesk', sans-serif;
	}
</style>
