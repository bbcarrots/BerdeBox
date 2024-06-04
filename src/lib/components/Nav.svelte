<script lang="ts">
	import { Icon, Cog8Tooth, ChevronRight } from 'svelte-hero-icons';
	import { page } from '$app/stores';
	import ProfilePhoto from './ProfilePhoto.svelte';

	$: activeUrl = $page.url.pathname;

	const activeClasses =
		'bg-bb-green-2 text-white font-medium rounded-[15px] text-sm px-8 py-3 text-center me-2 mb-2';
	const inactiveClasses =
		'bg-white text-bb-black font-medium rounded-[15px] text-sm px-8 py-3 text-center me-2 mb-2';
</script>

<div class="flex items-center justify-between">
	<div>
		<a href="/app/boxes">
			<button type="button" class={activeUrl === '/app/boxes' ? activeClasses : inactiveClasses}>
				<p>Boxes</p>
			</button>
		</a>
		<a href="/app/modules">
			<button type="button" class={activeUrl === '/app/modules' ? activeClasses : inactiveClasses}>
				<p>Modules</p>
			</button>
		</a>
	</div>

	<div class="flex items-center gap-2">
		<a href="/app/settings">
			<div class="w-[55px]">
				<ProfilePhoto></ProfilePhoto>
			</div>
		</a>
	</div>
</div>
