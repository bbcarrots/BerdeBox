<script lang="ts">
	import { page } from '$app/stores';
	import ControlButton from '$lib/components/ControlButton.svelte';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import Log from '$lib/components/Log.svelte';
	import { Icon, XMark } from 'svelte-hero-icons';
	import { Logs } from '$lib/stores/IO';
	import { onMount } from 'svelte';

	const boxID = Number($page.params.boxID);
	$: reversedLogs = [...$Logs].reverse();

	let isControl: boolean = true;
	let selectedLogIndex: Number = 0;
	let src: string;
	let datetime: Date;
	let message: string;
	let status: string;

	function handleLogClick(index: number) {
		selectedLogIndex = index;
		src = reversedLogs[index].imageURL;
		datetime = reversedLogs[index].datetime;
		message = reversedLogs[index].message;
		status = reversedLogs[index].status;
	}

	// -------------------------------------------------------- FIREBASE CODE
	import { firebaseDBFront } from '$lib/stores/FirebaseClient';
	import { onValue, ref, update } from 'firebase/database';

	let isDoorbellRung: boolean = false;

    const asyncTimeout = (ms: number) => {
        return new Promise((resolve) => {
            setTimeout(resolve, ms);
        });
    };

	async function handleDoorbell(snapshot) {
		if (snapshot.val()) {
			isDoorbellRung = true;
			update(ref($firebaseDBFront, 'berdebox1/output'), {take_photo: true});
            await asyncTimeout(2000)
            update(ref($firebaseDBFront, 'berdebox1/input'), {doorbell_button_is_pressed: false})
		}
	}

	onMount(() => {
		src = reversedLogs[0]?.imageURL;
		datetime = reversedLogs[0]?.datetime;
		message = reversedLogs[0]?.message;
		status = reversedLogs[0]?.status;

		// -------------------------------------------------------- FIREBASE CODE
		onValue(ref($firebaseDBFront, 'berdebox1/input/doorbell_button_is_pressed'), handleDoorbell);
	});

	const tabbarActiveClasses = 'p-3 bg-white rounded-[15px] m-2 shadow-sm';
	const tabbarInactiveClasses = 'p-3 m-2';

	// -------------------------------------------------------- FIREBASE CODE
	let mainLockIsOpen: boolean = false;
	let cashBoxes: { [key: string]: boolean } = {
		cb1: false,
		cb2: false,
		cb3: false
	};

	async function handleMainLockOpen() {
		/* Handles Open Main Lock event from Main Lock button by sending a PATCH request
        with payload requirement: boxCode. */

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

		await asyncTimeout(3000);
		// console.log('Main Lock should be locked again (UI side only, not DB side)!');
		mainLockIsOpen = false;
	}

	async function handleOpenCashBox(cashBoxID: number) {
		/* Handles Open Cash Box Lock event from Cash Box Lock button by sending a PATCH request
        with payload requirement: cashBoxCode. */

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

		await asyncTimeout(3000);
		// console.log('Cash Box Lock should be locked again (UI side only, not DB side)!');
		cashBoxes['cb' + cashBoxID.toString()] = false;
	}
</script>

<section class="h-screen max-h-[screen] overflow-hidden">
	<!-- header -->
	<div class="flex items-center justify-between p-4">
		<div></div>
		<h4>Mailbox</h4>
		<a href="/boxes" class="right-2">
			<Icon src={XMark} solid size="20" />
		</a>
	</div>

	<!-- content -->
	<div class="grid grid-cols-1 md:grid-cols-5 h-[calc(100%-60px)] m-0 md:m-4 md:h-[90%]">
		<!-- box preview -->
		<div class="col-span-3 mb-4 md:mr-4">
			<BoxPreview {src} {datetime} {message} {status}></BoxPreview>
		</div>

		<div class="grid grid-rows-7 md:col-span-2 md:grid-rows-10 w-full h-full">
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
				class="min-h-full h-0 row-span-6 md:row-span-9 bg-white items-center justify-center rounded-t-[15px] md:rounded-b-[15px] md:mb-4 p-4 w-full mt-2"
			>
				{#if isControl}
					<span class="grid h-full gap-4 grid-cols-2">
						<ControlButton
							boxType={'Mailbox 1'}
							bind:open={mainLockIsOpen}
							on:held={handleMainLockOpen}
						></ControlButton>
						<ControlButton
							boxType={'Cashbox 1'}
							bind:open={cashBoxes.cb1}
							on:held={() => handleOpenCashBox(1)}
						></ControlButton>
						<ControlButton
							boxType={'Cashbox 2'}
							bind:open={cashBoxes.cb2}
							on:held={() => handleOpenCashBox(2)}
						></ControlButton>
						<ControlButton
							boxType={'Cashbox 3'}
							bind:open={cashBoxes.cb3}
							on:held={() => handleOpenCashBox(3)}
						></ControlButton>
					</span>
				{:else}
					<div class="h-full overflow-auto overflow-y-scroll">
						{#each reversedLogs as log, index}
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
					</div>
				{/if}
			</div>
		</div>
	</div>
</section>
