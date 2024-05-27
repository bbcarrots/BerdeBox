<script lang="ts">
	import { Icon, Cog8Tooth } from 'svelte-hero-icons';
	import { page } from '$app/stores';

	$: activeUrl = $page.url.pathname;

	const activeClasses =
		'bg-[#578751] text-white font-medium rounded-[15px] text-sm px-8 py-3 text-center me-2 mb-2';
	const inactiveClasses =
		'bg-white text-bb-dark-green font-medium rounded-[15px] text-sm px-8 py-3 text-center me-2 mb-2';
</script>

<div class="flex items-center justify-between">
	<div>
		<a href="/boxes">
			<button type="button" class={activeUrl === '/boxes' ? activeClasses : inactiveClasses}>
				<p>Boxes</p>
			</button>
		</a>
		<a href="/modules">
			<button type="button" class={activeUrl === '/modules' ? activeClasses : inactiveClasses}>
				<p>Modules</p>
			</button>
		</a>
	</div>

	<a href="/settings">
		<button class="text-bb-dark-green">
			<Icon src={Cog8Tooth} solid size="32" />
		</button>
	</a>
</div>
