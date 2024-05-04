<script lang="ts">
    export let src;

</script>

<button on:click>
    <div class="w-[500px] h-[250px] bg-gray-100">
        {#if src == null}
            No one rang the doorbell
        {:else}
            <img src={src} alt="preview"/>
        {/if}
    </div>
</button>