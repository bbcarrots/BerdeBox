<script lang="ts">
	import { goto } from "$app/navigation";
    import BoxPreview from "$lib/components/BoxPreview.svelte";
    import { getImage } from "$lib/firebase/storage";
	import { onMount } from "svelte";
    import Nav from "$lib/components/Nav.svelte";

    let src: string | null = ""
    
    // handle route for "multiple boxes" in the future
    async function handleRoute(boxNum: number){
        goto(`/boxes/${boxNum}`);
    }

    onMount(() => {
        getImage('delivery_man.jpeg').then(url => {
            src = url;
        });
    });
</script>

<body class="grid gap-4 m-4">
    <section class="grid gap-4">
        <h2 class="text-bb-black">BerdeBox</h2>
        <Nav></Nav>
    </section>
    <section class="grid grid-cols-1 gap-4 md:grid-cols-2 w-full">
        <BoxPreview {src} on:click={()=>handleRoute(1)}/>
        <BoxPreview src={null} on:click={()=>handleRoute(1)}/>
    </section>
</body>
