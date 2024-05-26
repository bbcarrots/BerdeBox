<script lang="ts">
	import { onMount } from 'svelte';
	import '../app.css';
	import { auth } from '$lib/firebase/auth';
	import { onAuthStateChanged } from 'firebase/auth';
	import { UserStore } from '$lib/stores/User';
	import { getUserbyID, addUser } from '$lib/firebase/firestore';
	import { goto } from '$app/navigation';
	import { updateBoxesStore } from '$lib/utils/storeFunctions';
	import Loader from '$lib/components/Loader.svelte';
	import { loading, retrievingBoxes } from '$lib/stores/Page';
	import { page } from '$app/stores';

	if ($page.url.pathname == '/') {
		loading.set(true);
	}

	onMount(() => {
		// HANDLES AUTHENTICATION
		onAuthStateChanged(auth, async (user) => {
			// If authentication succeeds
			if (user) {
				retrievingBoxes.set(true);
				let validUser = await getUserbyID(user.uid);

				// If the user exists, update the user store
				// then update the boxes store based on user inforation
				// then redirect to /boxes
				if (validUser !== null && user !== null) {
					UserStore.set({
						name: user.displayName,
						profilePhoto: user.photoURL,
						uid: user.uid,
						notifToken: validUser?.notifToken,
						boxes: validUser?.berdeboxes,
						notifsPermitted: validUser?.notifsPermitted
					});

					await updateBoxesStore($UserStore.boxes);
					retrievingBoxes.set(false);
					goto('/boxes');
				}

				// If the user doesn't exist yet
				// add a new user document to firestore
				// then retrieve the user and update the user store
				// then reroute to /boxes
				else {
					addUser(user.uid);
					retrievingBoxes.set(true);

					let validUser = await getUserbyID(user.uid); // Await the promise

					UserStore.set({
						name: user.displayName,
						profilePhoto: user.photoURL,
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
				loading.set(false);
				// Handle the case where there's no user
				console.log('No user is signed in');
			}
		});
	});
</script>

{#if $loading == false}
	<body class="h-svh bg-[#F0F5F4]">
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
