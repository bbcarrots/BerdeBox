<script lang="ts">
	import { Icon, ShieldCheck } from 'svelte-hero-icons';
	import { loading } from '$lib/stores/Page';

	export let src;
	export let datetime;
	export let message;
	export let status;
</script>

<button on:click class="w-full h-full min-h-[250px] rounded-[15px] overflow-hidden bg-white -py-10">
	<div class="flex items-center relative w-full h-full">
		{#if $loading}
			<div class="flex flex-row items-center justify-center w-full aspect-video">
				<span class="grid gap-0 justify-items-center items-center text-center">
					<img class="w-[100px]" src={'../icons/boxes-loading.gif'} alt="loading box animation" />
				</span>
			</div>
		{:else if src == null}
			<!-- Empty State -->
			<div class="flex flex-row items-center justify-center w-full aspect-video">
				<span class="grid gap-0 justify-items-center items-center text-center text-[#ABABAB]">
					<Icon src={ShieldCheck} solid size="48" />
					<p>All deliveries secured!</p>
				</span>
			</div>
		{:else}
			<!-- Image State -->
			<div class="w-full h-full">
				<!-- Header -->
				<div
					class="absolute w-full bg-gradient-to-b from-black/[.70] to-transparent text-left text-white p-4 rounded"
				>
					<h6 class="left-0">MailBox {datetime.toLocaleString()}</h6>
				</div>
				<!-- Notification -->
				{#if status == 'success'}
					<div
						class="bg-[#78A36E]/[0.80] absolute bottom-2 left-4 px-3 py-2 text-white rounded-full"
					>
						<h6>{message}</h6>
					</div>
				{:else if status == 'alert'}
					<div
						class="bg-yellow-600/[0.80] absolute bottom-2 left-4 px-3 py-2 text-white rounded-full"
					>
						<h6>{message}</h6>
					</div>
				{:else if status == 'error'}
					<div class="bg-red-600/[0.80] absolute bottom-2 left-4 px-3 py-2 text-white rounded-full">
						<h6>{message}</h6>
					</div>
				{:else}
					<div></div>
				{/if}

				<!-- Image -->
				<img class=" aspect-video object-cover w-full h-full" {src} alt="preview" />
			</div>
		{/if}
	</div>
</button>
