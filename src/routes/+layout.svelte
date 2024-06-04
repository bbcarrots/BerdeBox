<script lang="ts">
	import '../landing.css';
	let scrollY: number;
</script>

<svelte:window bind:scrollY />

<div class="h-svh bg-[#F0F5F4]">
	<div class="">
		<slot />
	</div>
</div>

<style>
	@import url('https://fonts.googleapis.com/css2?family=Inter:wght@100..900&family=Montserrat:ital,wght@0,100..900;1,100..900&family=Space+Grotesk:wght@300..700&display=swap');
	@font-face {
		font-family: 'ProximaNova-Light';
		src: url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.eot');
		src: url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.eot?#iefix')
				format('embedded-opentype'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.woff2') format('woff2'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.woff') format('woff'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.ttf') format('truetype'),
			url('https://db.onlinewebfonts.com/t/90b3680786db32f9b5c112d1537677db.svg#ProximaNova-Light')
				format('svg');
	}
	:global(body) {
		font-family: 'Montserrat';
	}
</style>