<script>
	import { UserStore } from '$lib/stores/User';
</script>

<div class="flex gap-4 pt-4 my-4 justify-between items-center">
	<div class="text-bb-black">
		<h3>BerdeBox</h3>
		<p class="">Track your deliveries</p>
	</div>
	<img
		class="rounded-full w-[60px] h-[60px]"
		src={$UserStore.profilePhoto}
		alt="user profile"
		referrerpolicy="no-referrer"
	/>
</div>
