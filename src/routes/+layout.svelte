<script lang="ts">
	import { onMount } from 'svelte';
	import '../app.css';
	import { auth } from '$lib/firebase/auth';
	import { onAuthStateChanged } from 'firebase/auth';
	import { UserStore } from '$lib/stores/User';
	import { getUserbyID, addUser, getBoxByRef } from '$lib/firebase/firestore';
	import { goto } from '$app/navigation';
	import { Boxes } from '$lib/stores/IO';
	import { getImages } from '$lib/firebase/storage';

	//todo: update in the future to get all of the images in a folder
	onMount(() => {
		//input the id and the date string
		//to-do: make this unhardcoded

		onAuthStateChanged(auth, async (user) => {
			// Mark this function as async
			if (user) {
				// check if the user is a valid user already
				let validUser = await getUserbyID(user.uid); // Await the promise

				// if the user already exists, load the user info and get boxes
				if (validUser !== null) {
					// subscribe to the notif
					UserStore.set({
						uid: user.uid,
						notifToken: validUser?.notifToken,
						boxes: validUser?.berdeboxes
					});

					//populate the boxes store
					$UserStore.boxes.forEach(async (boxRef: any) => {
						// obtain the box
						let box: any = await getBoxByRef(boxRef);
						const logs = await getImages(box.id);
						let reversedLogs = [...logs].reverse();

						console.log(box);
						// update the boxes store
						Boxes.update((currentBoxes) => [
							...currentBoxes,
							{
								id: box.id,
								logs: reversedLogs
							}
						]);
					});

					goto('/boxes');
				}
				// if the user doesnt exist yet, add a new user to firestore
				else {
					//add the user
					addUser(user.uid);

					// obtain the user
					let validUser = await getUserbyID(user.uid); // Await the promise

					// store the user information
					UserStore.set({
						uid: user.uid,
						notifToken: validUser?.notifToken,
						boxes: validUser?.berdeboxes
					});

					console.log('Store', $UserStore);
					goto('/boxes');
				}
			} else {
				// Handle the case where there's no user
				console.log('No user is signed in');
			}
		});
	});
</script>

<body class="bg-[#EEF2F5]">
	<slot />
</body>

<style>
	@import url('https://fonts.googleapis.com/css2?family=Familjen+Grotesk:ital,wght@0,400..700;1,400..700&display=swap');

	:global(body) {
		font-family: 'Familjen Grotesk', sans-serif;
	}
</style>
