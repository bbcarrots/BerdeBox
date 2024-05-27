<script>
    import { loadingProfilePhoto } from "$lib/stores/Page";
    import { UserStore } from "$lib/stores/User";
    import SmallLoader from "./SmallLoader.svelte";
</script>

<div class={$loadingProfilePhoto ? 'hidden w-full h-full' : 'block'}>
    <img
        on:load={() => {
            loadingProfilePhoto.set(false);
        }}
        class="rounded-full w-full h-full"
        src={$UserStore.profilePhoto}
        alt="user profile"
        referrerpolicy="no-referrer"
    />
    {#if $loadingProfilePhoto}
        <div class="bg-white flex rounded-full w-[60px] h-[60px] items-center justify-center">
            <SmallLoader />
        </div>
    {/if}
</div>