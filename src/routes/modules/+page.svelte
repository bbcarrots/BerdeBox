<script>
	import Nav from '$lib/components/Nav.svelte';
	import HomeHeader from '$lib/components/HomeHeader.svelte';
</script>

<body class="grid gap-4">
	<section class="grid gap-4 mx-4">
		<HomeHeader></HomeHeader>
		<Nav></Nav>
	</section>
	<section class="grid grid-cols-1 gap-4 md:grid-cols-2 w-full"></section>
</body>
