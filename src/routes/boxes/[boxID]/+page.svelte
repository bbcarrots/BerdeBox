<script lang="ts">
	import { page } from '$app/stores';
	import ControlButton from '$lib/components/ControlButton.svelte';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import Log from '$lib/components/Log.svelte';
	import { Icon, ChevronLeft } from 'svelte-hero-icons';
	import { Boxes } from '$lib/stores/IO';
	import { onMount } from 'svelte';
	import { fly } from 'svelte/transition';
	import { scale } from 'svelte/transition';

	// --------------------------------------
	const boxID = Number($page.params.boxID) - 1;
	let mainLockIsOpen: boolean = false;
	let cashBoxes: { [key: string]: boolean } = {
		cb1: false,
		cb2: false,
		cb3: false
	};

	let isControl: boolean = true;
	let selectedLogIndex: Number = 0;
	let src: string;
	let datetime: Date;
	let message: string;
	let status: string;
	let takePhoto: boolean = false;

	function handleLogClick(index: number) {
		selectedLogIndex = index;
		src = $Boxes[boxID].logs[index].imageURL;
		datetime = $Boxes[boxID].logs[index].datetime;
		message = $Boxes[boxID].logs[index].message;
		status = $Boxes[boxID].logs[index].status;
	}

	// -------------------------------------------------------- FIREBASE CODE
	import { firebaseDBFront } from '$lib/stores/FirebaseClient';
	import { onValue, ref, update } from 'firebase/database';

	onValue(ref($firebaseDBFront, 'berdebox1/output/'), async (snapshot: any) => {
		const output = snapshot.val();

		console.log(output);

		takePhoto = output.take_photo;

		if (output.main_lock_is_open == false && output.take_photo == false) {
			mainLockIsOpen = false;
		}
		if (output.modules.cb1_is_open == false && output.take_photo == false) {
			cashBoxes.cb1 = false;
		}
		if (output.modules.cb2_is_open == false && output.take_photo == false) {
			cashBoxes.cb2 = false;
		}
		if (output.modules.cb2_is_open == false && output.take_photo == false) {
			cashBoxes.cb3 = false;
		}
	});

	let isDoorbellRung: boolean = false;

	const asyncTimeout = (ms: number) => {
		return new Promise((resolve) => {
			setTimeout(resolve, ms);
		});
	};

	async function handleDoorbell(snapshot: any) {
		if (snapshot.val()) {
			isDoorbellRung = true;
			update(ref($firebaseDBFront, 'berdebox1/output'), { take_photo: true });
			await asyncTimeout(2000);
			update(ref($firebaseDBFront, 'berdebox1/input'), { doorbell_button_is_pressed: false });
		}
	}

	$: {
		if ($Boxes.length !== 0) {
			src = $Boxes[boxID].logs[0]?.imageURL;
			datetime = $Boxes[boxID].logs[0]?.datetime;
			message = $Boxes[boxID].logs[0]?.message;
			status = $Boxes[boxID].logs[0]?.status;
		}
	}

	onMount(async () => {
		// -------------------------------------------------------- FIREBASE CODE
		onValue(ref($firebaseDBFront, 'berdebox1/input/doorbell_button_is_pressed'), handleDoorbell);
	});

	const tabbarActiveClasses =
		' hit-area transition ease-in-out p-3 bg-white rounded-[15px] m-2 shadow-sm';
	const tabbarInactiveClasses = 'hit-area p-3 m-2';

	// -------------------------------------------------------- FIREBASE CODE

	async function handleMainLockOpen() {
		/* Handles Open Main Lock event from Main Lock button by sending a PATCH request
        with payload requirement: boxCode. */
		mainLockIsOpen = true;
		const payload = { boxCode: 'berdebox' + $page.params.boxID.toString() };

		// console.log('Main Lock should be open!');
		const response = await fetch('../../api/output/mainlock', {
			method: 'PATCH',
			body: JSON.stringify(payload),
			headers: {
				'content-type': 'application/json'
			}
		});
		// console.log(await response.json());

		// await asyncTimeout(5000);
		// console.log('Main Lock should be locked again (UI side only, not DB side)!');
	}

	async function handleOpenCashBox(cashBoxID: number) {
		/* Handles Open Cash Box Lock event from Cash Box Lock button by sending a PATCH request
        with payload requirement: cashBoxCode. */
		cashBoxes['cb' + cashBoxID.toString()] = true;

		const payload = {
			boxCode: 'berdebox' + $page.params.boxID.toString(),
			cashBoxCode: 'cb' + cashBoxID.toString()
		};

		// console.log('Cash Box Lock should be open!');
		const response = await fetch('../../api/output/cashbox', {
			method: 'PATCH',
			body: JSON.stringify(payload),
			headers: {
				'content-type': 'application/json'
			}
		});
		// console.log(await response.json());

		// await asyncTimeout(3000);
		// console.log('Cash Box Lock should be locked again (UI side only, not DB side)!');
	}
</script>

<section class="h-svh relative">
	<!-- header -->
	<div class="sticky top-0 z-10 bg-[#F0F5F4] flex items-center justify-between px-4 py-6">
		<a href="/boxes" class="left-2">
			<Icon src={ChevronLeft} solid size="20" />
		</a>
		{#key src}
			<div in:scale class="bg-white py-2 px-4 rounded-full flex gap-2 items-center">
				{#if status == 'success'}
					<div class="w-3 h-3 bg-[#78D96C] rounded-full"></div>
				{:else if status == 'error'}
					<div class="w-3 h-3 bg-red-500 rounded-full"></div>
				{:else}
					<div class="w-3 h-3 bg-yellow-500 rounded-full"></div>
				{/if}
				<p class="text-bb-dark-green">{message}</p>
			</div>
		{/key}

		<div></div>
	</div>
	<!-- content -->
	<div
		class="grid gap-2 grid-cols-1 grid-rows-8 md:grid-rows-1 md:grid-cols-5 h-[90%] m-0 md:m-4 md:h-[85%]"
	>
		<!-- box preview -->
		<div transition:scale class="md:col-span-3 row-span-3 md:mr-4">
			<BoxPreview {src} {datetime} {message} status={'none'}></BoxPreview>
		</div>

		<div
			transition:fly={{ x: 0, y: 3000 }}
			class="grid row-span-5 grid-rows-7 md:col-span-2 md:grid-rows-10 w-full h-full overflow-hidden"
		>
			<!-- tabbar buttons -->
			<div
				id="tabbar"
				class="grid row-span-1 grid-cols-2 bg-[#DEE3E9] mx-8 md:row-span-1 md:mx-0 rounded-[20px] h-[70px]"
			>
				<button
					class={isControl === true ? tabbarActiveClasses : tabbarInactiveClasses}
					on:click={() => {
						isControl = true;
					}}
				>
					<p>Controls</p>
				</button>
				<button
					class={isControl === false ? tabbarActiveClasses : tabbarInactiveClasses}
					on:click={() => {
						isControl = false;
					}}
				>
					<p>Logs</p>
				</button>
			</div>

			<!-- tabbar content -->
			<div
				class="h-full row-span-6 md:row-span-9 bg-white items-center justify-center rounded-t-[15px] md:rounded-b-[15px] md:mb-4 p-4 w-full mt-2"
			>
				{#if isControl}
					<span class="grid h-full gap-4 grid-cols-2">
						<ControlButton
							boxType={'Mailbox 1'}
							bind:open={mainLockIsOpen}
							on:held={handleMainLockOpen}
							isDisabled={takePhoto}
						></ControlButton>
						<ControlButton
							boxType={'Cashbox 1'}
							bind:open={cashBoxes.cb1}
							on:held={() => handleOpenCashBox(1)}
							isDisabled={takePhoto}
						></ControlButton>
						<ControlButton
							boxType={'Cashbox 2'}
							bind:open={cashBoxes.cb2}
							on:held={() => handleOpenCashBox(2)}
							isDisabled={takePhoto}
						></ControlButton>
						<ControlButton
							boxType={'Cashbox 3'}
							bind:open={cashBoxes.cb3}
							on:held={() => handleOpenCashBox(3)}
							isDisabled={takePhoto}
						></ControlButton>
					</span>
				{:else}
					<div class="h-full overflow-auto overflow-y-scroll">
						{#if $Boxes.length !== 0}
							{#each $Boxes[boxID].logs as log, index}
								{#if index === selectedLogIndex}
									<Log {log} selected={true} />
								{:else}
									<Log
										on:click={() => {
											handleLogClick(index);
										}}
										{log}
										selected={false}
									/>
								{/if}
							{/each}
						{/if}
					</div>
				{/if}
			</div>
		</div>
	</div>
</section>
