<script lang="ts">
	import { goto } from '$app/navigation';
	import BoxPreview from '$lib/components/BoxPreview.svelte';
	import Nav from '$lib/components/Nav.svelte';
	import { ImageURLs } from '$lib/stores/IO';

	// handle route for "multiple boxes" in the future
	async function handleRoute(boxNum: number) {
		goto(`/boxes/${boxNum}`);
	}
</script>

<body class="grid gap-4 m-4">
	<section class="grid gap-4">
		<h2 class="text-bb-black">BerdeBox</h2>
		<Nav></Nav>
	</section>
	<section class="grid grid-cols-1 gap-4 md:grid-cols-2 w-full">
		<BoxPreview src={$ImageURLs[0]} on:click={() => handleRoute(1)} />
		<BoxPreview src={null} on:click={() => handleRoute(2)} />
	</section>
</body>
