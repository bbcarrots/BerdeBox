<script lang="ts">
	import { goto } from '$app/navigation';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import Nav from '$lib/components/Nav.svelte';
	import { Icon, ChevronLeft, Plus } from 'svelte-hero-icons';
	import { onMount } from 'svelte';
	import { getImages } from '$lib/firebase/storage';
	import { Boxes } from '$lib/stores/IO';
	import { getBerdeBoxes } from '$lib/firebase/firestore';
	import { UserStore } from '$lib/stores/User';
	import { getBoxByRef, updateUserBoxes } from '$lib/firebase/firestore';

	let openCodeForm = false;

	// handle route for "multiple boxes" in the future
	async function handleRoute(boxNum: number) {
		goto(`/boxes/${boxNum}`);
	}

	let boxCode: string;
	async function handleSubmit() {
		initializeBox(Number(boxCode));
		openCodeForm = false;
	}

	onMount(() => {
		console.log();
	});

	//box to initialize it in the UI
	export async function initializeBox(id: number) {
		// check if the box exits
		let berdeBoxes = await getBerdeBoxes();

		berdeBoxes.forEach((box) => {
			//if the box exists
			if (box.data().id == id) {
				// add it to the user's firestore
				updateUserBoxes($UserStore.uid, box.ref);

				// load the boxes again to update
				$UserStore.boxes = [];
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
			}
			// if the box doesn't exist
			else {
				console.log('Box does not exist');
			}
		});
	}
</script>

<section class="h-calc([100%-20px]) max-h-screen {openCodeForm ? 'overflow-hidden' : ''}">
	{#if openCodeForm}
		<!-- Code Form Popup -->
		<div class="z-20 grid grid-cols-1 w-full h-screen bg-[#EEF2F5] absolute">
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
					<form class="flex flex-col md:justify-normal gap-8 justify-between h-full p-8">
						<div class="grid gap-4">
							<label for="boxID">
								<h4>Box Code</h4>
							</label>
							<input
								bind:value={boxCode}
								class="p-4 rounded-[10px]"
								type="text"
								id="boxID"
								name="boxID"
							/>
						</div>

						<input
							on:click={handleSubmit}
							type="submit"
							value="Add Box"
							class="flex gap-4 bg-bb-black text-white p-4 rounded-[15px] items-center justify-center"
						/>
					</form>
				</div>
			</div>
		</div>
	{/if}

	<body class="grid gap-4 m-4">
		<section class="grid gap-4">
			<h2 class="text-bb-black">BerdeBox</h2>
			<Nav></Nav>
		</section>
		<section class="grid grid-cols-1 gap-4 md:grid-cols-2 w-full">
			{#each $Boxes as box}
				<BoxPreview
					src={box.logs[0]?.imageURL}
					message={box.logs[0]?.message}
					status={box.logs[0]?.status}
					datetime={box.logs[0]?.datetime}
					on:click={() => handleRoute(box.id)}
				/>
			{/each}
			<button
				class="w-full h-full bg-white rounded-[15px] min-h-[250px]"
				on:click={() => {
					openCodeForm = true;
				}}
			>
				<div class="flex flex-col items-center space-y-2">
					<Icon src={Plus} solid size="36" />
					<p>Add a new BerdeBox</p>
				</div>
			</button>
		</section>
	</body>
</section>
