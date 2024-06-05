<script lang="ts">
	export let scrollY: number;

	function handleAnchorClick(event: any) {
		event.preventDefault();
		const link = event.currentTarget;
		const anchorId = new URL(link.href).hash.replace('#', '');
		const anchor = document.getElementById(anchorId);
		window.scrollTo({
			top: anchor?.offsetTop,
			behavior: 'smooth'
		});
	}
</script>

<div
	class={scrollY < 100 ? ' bg-red-100/[.80] text-white ' : ' bg-white/[.80] text-bb-darkest-green'}
>
	<header
		class="flex items-center justify-between px-[15%] py-2 fixed w-full z-50 backdrop-blur-md"
	>
		<div class="flex items-center">
			<img src="/logos/berdeBoxLogo3.png" alt="Hero Icon" class="w-5 h-5 mr-4" />
			<h1 class="text-xl font-bold">BerdeBox</h1>
		</div>
		<div class="flex space-x-4">
			<a href="#features" on:click={handleAnchorClick}>
				<button type="button" class="px-4 py-2 rounded">
					<h6>Features</h6>
				</button>
			</a>
			<a href="#modules" on:click={handleAnchorClick}>
				<button type="button" class="px-4 py-2 rounded">
					<h6>Modules</h6>
				</button>
			</a>
		</div>
	</header>
</div>
