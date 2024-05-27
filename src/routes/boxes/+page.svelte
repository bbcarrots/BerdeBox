<script lang="ts">
	import { goto } from '$app/navigation';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import Nav from '$lib/components/Nav.svelte';
	import { Icon, ChevronLeft, Plus } from 'svelte-hero-icons';
	import { Boxes } from '$lib/stores/IO';
	import { getBerdeBoxes } from '$lib/firebase/firestore';
	import { UserStore } from '$lib/stores/User';
	import { updateUserBoxes, getUserbyID } from '$lib/firebase/firestore';
	import { updateBoxesStore, isBoxInUserBoxes } from '$lib/utils/storeFunctions';
	import { retrievingBoxes } from '$lib/stores/Page';
	import HomeHeader from '$lib/components/HomeHeader.svelte';
	import { fly } from 'svelte/transition';

	let isRetrievingBoxes: boolean;
	$: {
		isRetrievingBoxes = $retrievingBoxes;
	}

	// ADD BOX VARIABLES
	let openCodeForm = false;
	let boxID: string;

	// BUTTON ACTIONS
	async function handleRoute(boxNum: number) {
		goto(`/boxes/${boxNum}`);
	}

	async function handleSubmit(event: Event) {
		event.preventDefault();

		retrievingBoxes.set(true);
		await initializeBox(Number(boxID));
		openCodeForm = false;

		setTimeout(() => {
			retrievingBoxes.set(false);
		}, 2000);
	}

	// ADD BOX FUNCTIONS
	async function initializeBox(boxID: number) {
		// Check if the id exists in the current box
		// Obtain all of the boxes
		let berdeBoxes = await getBerdeBoxes();

		// Loop through each available berde box
		berdeBoxes.forEach((box) => {
			//Check if the box id exists in berde boxes firestore
			if (box.data().id == boxID) {
				// Check if the box id exists in the user berde boxes
				// Add it to the user's firestore if box id not in user
				(async () => {
					if (await isBoxInUserBoxes(boxID, $UserStore.boxes)) {
						console.log('Box is in user!');
					} else {
						await updateUserStore(box);
						await updateBoxesStore($UserStore.boxes);
					}
				})();
			}
			// If the box doesn't exist
			else {
				console.log('Box does not exist');
			}
		});
	}

	async function updateUserStore(box: any) {
		await updateUserBoxes($UserStore.uid, box.ref);
		let validUser = await getUserbyID($UserStore.uid);

		UserStore.set({
			name: $UserStore?.name,
			profilePhoto: $UserStore.profilePhoto,
			uid: validUser?.uid,
			notifToken: validUser?.notifToken,
			boxes: validUser?.berdeboxes,
			notifsPermitted: validUser?.notifsPermitted
		});
		return true;
	}
</script>

<section class="h-svh {openCodeForm ? 'overflow-hidden' : ''}">
	{#if openCodeForm}
		<!-- Code Form Popup -->
		<div
			in:fly={{ x: 3000, y: 0 }}
			class="z-20 grid grid-cols-1 w-full h-svh bg-[#EEF2F5] absolute"
		>
			<!-- Form Content -->
			<div class="flex flex-col items-center">
				<!-- Header -->
				<div class="flex items-center justify-between p-4 w-full my-4">
					<button
						on:click={() => {
							openCodeForm = false;
						}}
						class="right-2"
					>
						<Icon src={ChevronLeft} solid size="20" />
					</button>
				</div>

				<!-- Form Items -->
				<div
					class="grid w-full md:max-w-[600px] h-[calc(100%-60px)] m-0 md:absolute md:h-[90%] md:content-center"
				>
					<form class="flex flex-col md:justify-normal gap-8 h-full p-8">
						<div class="grid gap-4">
							<label for="boxID">
								<h4>Box Code</h4>
							</label>
							<input
								bind:value={boxID}
								class="p-4 rounded-[10px]"
								type="text"
								id="boxID"
								name="boxID"
							/>
						</div>

						<input
							on:click={(event) => {
								handleSubmit(event);
							}}
							type="submit"
							value="Add Box"
							class="flex gap-4 bg-bb-black text-white p-4 rounded-[15px] items-center justify-center hover:cursor-pointer"
						/>
					</form>
				</div>
			</div>
		</div>
	{/if}

	<body class="grid gap-4 mx-4">
		<section class="grid gap-4">
			<HomeHeader></HomeHeader>

			<Nav></Nav>
		</section>
		<section in:fly={{ x: -3000, y: 0 }} class="grid grid-cols-1 gap-4 md:grid-cols-2 w-full">
			{#key $Boxes}
				{#if isRetrievingBoxes}
					<div
						class="flex flex-row items-center justify-center w-full h-full bg-white rounded-[15px] min-h-[250px] md:min-h-[400px]"
					>
						<span class="grid gap-0 justify-items-center items-center text-center">
							<img class="w-[100px]" src="icons/boxes-loading.gif" alt="loading box animation" />
							<p>Fetching your boxes</p>
						</span>
					</div>
				{:else}
					{#each $Boxes as box}
						<span>
							<BoxPreview
								src={box.logs[0]?.imageURL}
								message={box.logs[0]?.message}
								status={box.logs[0]?.status}
								datetime={box.logs[0]?.datetime}
								on:click={() => handleRoute(box.id)}
							/>
						</span>
					{/each}
				{/if}
			{/key}
			<button
				class="w-full h-full bg-white rounded-[15px] min-h-[250px] md:min-h-[400px]"
				on:click={() => {
					openCodeForm = true;
				}}
			>
				<div class="flex flex-col items-center space-y-2 text-bb-dark-green">
					<Icon src={Plus} solid size="44" />
					<h4>Add a new BerdeBox</h4>
				</div>
			</button>
		</section>
	</body>
</section>
