<script lang="ts">
	import { onMount } from 'svelte';
	import '../../app.css';
	import { auth } from '$lib/firebase/auth';
	import { onAuthStateChanged } from 'firebase/auth';
	import { UserStore } from '$lib/stores/User';
	import { getUserbyID, addUser } from '$lib/firebase/firestore';
	import { goto } from '$app/navigation';
	import { updateBoxesStore } from '$lib/utils/storeFunctions';
	import Loader from '$lib/components/Loader.svelte';
	import { loading, retrievingBoxes } from '$lib/stores/Page';
	import { page } from '$app/stores';
	import { onValue } from 'firebase/database';
	import { firebaseDBFront } from '$lib/stores/FirebaseClient';
	import { ref } from 'firebase/database';
	import { loginLoading } from '$lib/stores/Page';

	if ($page.url.pathname == '/app') {
		loginLoading.set(true);
	}
	loading.set(true);

	onMount(() => {
		// HANDLES AUTHENTICATION
		onAuthStateChanged(auth, async (user) => {
			// If authentication succeeds
			loginLoading.set(true);

			if (user) {
				onValue(ref($firebaseDBFront, 'berdebox1/output/take_photo'), async (snapshot: any) => {
					const take_photo = snapshot.val();

					// reload the logs when the take_photo is false
					if (take_photo == false) {
						await updateBoxesStore($UserStore.boxes);
					}
				});

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
					loading.set(false);

					retrievingBoxes.set(false);
					loginLoading.set(false);

					if ($page.url.pathname == '/app') {
						goto('/app/boxes');
					}
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
					if ($page.url.pathname == '/app') {
						goto('/app/boxes');
					}
				}
			}

			// If authentication fails
			else {
				loginLoading.set(false);
				loading.set(false);

				// Handle the case where there's no user
				console.log('No user is signed in');
			}
		});
	});
</script>

{#if $loginLoading == false}
	<body class="h-svh bg-[#F0F5F4]">
		<slot />
	</body>
{:else}
	<Loader></Loader>
{/if}

<style>
	@import url('https://fonts.googleapis.com/css2?family=Familjen+Grotesk:ital,wght@0,400..700;1,400..700&display=swap');
	@import url('https://fonts.googleapis.com/css2?family=Inter:wght@100..900&family=Montserrat:ital,wght@0,100..900;1,100..900&family=Space+Grotesk:wght@300..700&display=swap');
	@font-face {
		font-family: 'ProximaNova-Light';
		src: url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.eot');
		src:
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.eot?#iefix')
				format('embedded-opentype'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.woff2') format('woff2'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.woff') format('woff'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.ttf') format('truetype'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.svg#ProximaNova-Light')
				format('svg');
	}
	:global(body) {
		font-family: 'ProximaNova-Light';
	}
</style>
