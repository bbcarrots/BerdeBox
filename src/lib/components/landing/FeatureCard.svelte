<script>
	export let src;
	export let title;
	export let subtitle;
</script>

<div
	class="w-full bg-gradient-to-tr from-[#F8F8F8] hover:bg-[#F8F8F8] shadow-[0px 14px 50px 0px #E7E7E7] flex flex-col items-center p-[0.1rem] rounded-[25px] duration-300 ease-in-out"
>
	<!-- image -->
	<img {src} alt="berdebox feature" class="h-auto w-[60%] md:w-[90%]" />
	<!-- content -->
	<div class="flex flex-col items-center text-center text-bb-black w-[80%] gap-4 mb-20">
		<h5>{title}</h5>
		<p>{subtitle}</p>
	</div>
</div>
