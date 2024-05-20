<script lang="ts">
	import { page } from '$app/stores';
	import ControlButton from '$lib/components/ControlButton.svelte';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import { ImageURLs } from '$lib/stores/IO';
	import Log from '$lib/components/Log.svelte';
	import { Icon, XMark } from 'svelte-hero-icons';
	import { type log } from '$lib/utils/types';

	const boxID = Number($page.params.boxID);

	let isControl: boolean = true;

	let logs: log[] = [
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'success'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'alert'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'error'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'success'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'success'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'success'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'success'
		},
		{
			imageURL: $ImageURLs[0],
			datetime: new Date(),
			message: 'MailBox doorbell was pressed',
			status: 'success'
		}
	];

	function openBox() {
		console.log('action!');
	}

	const tabbarActiveClasses = 'p-3 bg-white rounded-[15px] m-2 shadow-sm';
	const tabbarInactiveClasses = 'p-3 m-2';

	// --------------------------------------------------------
    let mainLockIsOpen: boolean = false

	const asyncTimeout = (ms: number) => {
		return new Promise((resolve) => {
			setTimeout(resolve, ms);
		});
	};

	async function handleOpenMainLock() {
		/* Handles Open Main Lock event from Main Lock button by sending a PATCH request
        with payload requirement: boxCode. */

		const payload = { boxCode: 'berdebox' + $page.params.boxID.toString() };

        console.log("Main Lock should be open!")
		const response = await fetch('../../api/output/mainlock', {
			method: 'PATCH',
			body: JSON.stringify(payload),
			headers: {
				'content-type': 'application/json'
			}
		});
        console.log(await response.json())

        await asyncTimeout(3000);
        console.log("Main Lock should be locked again!")
        mainLockIsOpen = false
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
			<BoxPreview src={$ImageURLs}></BoxPreview>
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
				class="min-h-full h-0 row-span-6 md:row-span-9 bg-white items-center justify-center rounded-t-[15px] md:rounded-b-[15px] md:mb-4 p-4 w-full"
			>
				{#if isControl}
					<span class="grid h-full gap-4 grid-cols-2">
						<ControlButton boxType={'Mailbox 1'} bind:open={mainLockIsOpen} on:held={handleOpenMainLock}
						></ControlButton>
						<ControlButton boxType={'Cashbox 1'} open={false} on:held={openBox}></ControlButton>
						<ControlButton boxType={'Cashbox 2'} open={false} on:held={openBox}></ControlButton>
						<ControlButton boxType={'Cashbox 3'} open={true} on:held={openBox}></ControlButton>
					</span>
				{:else}
					<div class="h-full overflow-auto overflow-y-scroll">
						{#each logs as log, index}
							{#if index === 0}
								<Log {log} selected={true} />
							{:else}
								<Log {log} selected={false} />
							{/if}
						{/each}
					</div>
				{/if}
			</div>
		</div>
	</div>
</section>
