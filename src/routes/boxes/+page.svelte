<script lang="ts">
	import { goto } from "$app/navigation";
    import BoxPreview from "$lib/components/BoxPreview.svelte";
    import { getImage } from "$lib/firebase/storage";
	import { onMount } from "svelte";

    let src: string | null = ""
    
    // handle route for "multiple boxes" in the future
    async function handleRoute(boxNum: number){
        goto(`/boxes/${boxNum}`);
    }

    onMount(() => {
        console.log('Component mounted');
        getImage('delivery_man.jpeg').then(url => {
            src = url;
        });
    });
</script>

<BoxPreview {src} on:click={()=>handleRoute(1)}/>