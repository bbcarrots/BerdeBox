<script lang="ts">
	import { type log } from '$lib/utils/types';
	import { Icon, ExclamationCircle, CheckCircle, XCircle } from 'svelte-hero-icons';
	export let log: log;
	export let selected;
</script>

<button on:click>
	<div
		class="{selected == true
			? 'rounded-[15px] bg-[#F1F5F9]'
			: ''} grid gap-4 grid-cols-3 w-full p-2"
	>
		<img class="rounded-[10px] col-span-1" src={log.imageURL} alt="log" />
		<div class="grid col-span-2 items-center my-2">
			<h6 class="text-left">{log.datetime.toLocaleString()}</h6>
			<span class="flex items-center gap-1">
				{#if log.status == 'success'}
					<span class="text-green-500">
						<Icon src={CheckCircle} solid size={'15px'} />
					</span>
				{:else if log.status == 'alert'}
					<span class="text-yellow-400">
						<Icon src={ExclamationCircle} solid size={'15px'} />
					</span>
				{:else if log.status == 'error'}
					<span class="text-red-500">
						<Icon src={XCircle} solid size={'15px'} />
					</span>
				{/if}
				<p>{log.message}</p>
			</span>
		</div>
	</div>
</button>
