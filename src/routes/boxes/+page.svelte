<script lang="ts">
	import { goto } from '$app/navigation';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import Nav from '$lib/components/Nav.svelte';
	import { Logs } from '$lib/stores/IO';
	import { Icon, ChevronLeft } from 'svelte-hero-icons';

	let openCodeForm = true;

	$: reversedLogs = [...$Logs].reverse()
	// handle route for "multiple boxes" in the future
	async function handleRoute(boxNum: number) {
		goto(`/boxes/${boxNum}`);
	}
</script>

<section class="h-screen max-h-[screen] overflow-hidden">

	{#if openCodeForm}
		<!-- Code Form Popup -->
		<div class="z-20 grid grid-cols-1 w-full h-screen bg-[#EEF2F5] absolute">

			<!-- Form Content -->
			<div class="flex flex-col items-center">
				<!-- Header -->
				<div class="flex items-center justify-between p-4 w-full my-4">
					<button on:click={() => {openCodeForm = false}} class="right-2">
						<Icon src={ChevronLeft} solid size="20" />
					</button>
				</div>

				<!-- Form Items -->
				<div class="grid w-full md:max-w-[600px] h-[calc(100%-60px)] m-0 md:absolute md:h-[90%] md:content-center">
					<form class="flex flex-col md:justify-normal gap-8 justify-between h-full p-8">
						<div class="grid gap-4">
							<label for="boxID">
								<h4>Box Code</h4>
							</label>
							<input class="p-4 rounded-[10px]" type="text" id="boxID" name="boxID">
						</div>
						
						<button class="flex gap-4 bg-bb-black text-white p-4 rounded-[15px] items-center justify-center">
							Add Box
						</button>
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
			<BoxPreview
				src={reversedLogs[0]?.imageURL}
				message={reversedLogs[0]?.message}
				status={reversedLogs[0]?.status}
				datetime={reversedLogs[0]?.datetime}
				on:click={() => handleRoute(1)}
			/>
			<BoxPreview src={null} message={''} status={''} datetime={''} on:click={() => handleRoute(2)} />
		</section>
		<button on:click={() => {openCodeForm = true}}>Add box</button>
	</body>
	
</section>