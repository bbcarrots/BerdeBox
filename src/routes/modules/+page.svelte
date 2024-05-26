<script>
	import Nav from '$lib/components/Nav.svelte';
	import HomeHeader from '$lib/components/HomeHeader.svelte';
	import { fly } from 'svelte/transition';
	import { Icon, ArchiveBox } from 'svelte-hero-icons';
</script>

<body class="grid gap-4">
	<section class="grid gap-4 mx-4">
		<HomeHeader></HomeHeader>
		<Nav></Nav>
	</section>
	<section
		in:fly={{ x: -3000, y: 0 }}
		class="grid grid-cols-1 gap-4 w-full items-center justify-center"
	>
		<div class="flex flex-col items-center justify-center w-full h-[300px] text-gray-700 gap-4">
			<Icon src={ArchiveBox} outline size="64" />
			<h4 class="text-center">More modules will be added soon!</h4>
		</div>
	</section>
</body>
