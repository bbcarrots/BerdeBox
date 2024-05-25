<script>
	import { UserStore } from '$lib/stores/User';
	import SmallLoader from './SmallLoader.svelte';

	let loading = true;
</script>

<div class="flex gap-4 pt-4 my-4 justify-between items-center">
	<div class="text-bb-black">
		<h3>BerdeBox</h3>
		<p class="">Track your deliveries</p>
	</div>

	<div class={loading ? 'hidden' : 'block'}>
		<img
			on:load={() => {
				loading = false;
			}}
			class="rounded-full w-[60px] h-[60px]"
			src={$UserStore.profilePhoto}
			alt="user profile"
			referrerpolicy="no-referrer"
		/>
	</div>

	{#if loading}
		<div class="bg-white flex rounded-full w-[60px] h-[60px] items-center justify-center">
			<SmallLoader />
		</div>
	{/if}
</div>
