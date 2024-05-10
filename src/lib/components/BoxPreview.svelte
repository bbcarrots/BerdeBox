<script lang="ts">
    import { Icon, ShieldCheck } from "svelte-hero-icons";
    export let src;
    export let datetime = new Date();
    export let message = 'MailBox doorbell was rang';
</script>

<button on:click class="w-full h-full min-h-[250px] rounded-[15px] overflow-hidden bg-white -py-10">
    <div class="flex items-center relative w-full h-full">
        {#if src == null} 
        <!-- Empty State -->
            <div class="w-full">
                <div class="flex flex-col items-center text-[#ABABAB]"> 
                    <Icon src="{ShieldCheck}" solid size="48"/> 
                    <p>All deliveries secured!</p>
                </div>
            </div>
        {:else}
        <!-- Image State -->
        <div class="w-full h-full">
            <!-- Header -->
            <div class="absolute w-full bg-gradient-to-b from-black/[.70] to-transparent text-left text-white p-4 rounded">
                <h6 class="left-0">MailBox {datetime.toLocaleString()}</h6>
            </div>
            <!-- Notification -->
            <div class="bg-emerald-600/[0.80] absolute bottom-2 left-4 px-3 py-2 text-white rounded-full">
                <h6>{message}</h6>
            </div>
            <!-- Image -->
            <img class="object-cover w-full h-full" src={src} alt="preview"/>
        </div>
        {/if}
    </div>
</button>