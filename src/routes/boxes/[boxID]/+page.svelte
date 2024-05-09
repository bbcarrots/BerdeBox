<script lang="ts">
    import { page } from '$app/stores';
    import ControlButton from "$lib/components/ControlButton.svelte";
    import BoxPreview from '$lib/components/BoxPreview.svelte';
    import { ImageURLs } from '$lib/stores/IO';
    import { Icon, XMark } from 'svelte-hero-icons';
    const boxID = Number($page.params.boxID);

    let isControl: boolean = true;

    function openBox(){
        console.log("action!")
    }

    const tabbarActiveClasses="p-3 bg-white rounded-[15px] m-2 shadow-sm"
    const tabbarInactiveClasses="p-3 m-2"

</script>

<section class="h-screen">
    <!-- header -->
    <div class="flex items-center justify-between p-4">
        <div></div> 
        <h4>Mailbox</h4> 
        <a href="/boxes" class="right-2">
            <Icon src="{XMark}" solid size="20"/> 
        </a>
    </div>

    <!-- content -->
    <div class="grid grid-cols-1 md:grid-cols-5 h-[calc(100%-60px)] m-0 md:m-4 md:h-[90%]">
        <!-- box preview -->
        <div class="col-span-3 mb-4 md:mr-4">
            <BoxPreview src={$ImageURLs}></BoxPreview>
        </div>
        
        <div class="grid grid-rows-6 md:col-span-2 md:grid-rows-10 gap-2 w-full h-full">
            <!-- tabbar buttons -->
            <div id='tabbar' class="grid row-span-1 grid-cols-2 bg-[#DEE3E9] mx-8 md:row-span-1 md:mx-0 rounded-[20px] h-[70px]">
                <button class="{isControl === true ? tabbarActiveClasses : tabbarInactiveClasses}"
                    on:click={()=>{isControl = true}}
                >
                    <p>
                        Controls
                    </p>
                </button>
                <button class="{isControl === false ? tabbarActiveClasses : tabbarInactiveClasses}"
                    on:click={()=>{isControl = false}}
                >
                    <p>
                        Logs
                    </p>
                </button>
            </div>

            <!-- tabbar content -->
            <div class="grid row-span-5 md:row-span-9 bg-white grid-cols-2 items-center justify-center rounded-t-[15px] md:rounded-b-[15px] md:mb-4 p-4 gap-4 w-full">
                {#if isControl}
                        <ControlButton boxType={"Mailbox"} on:held={openBox}></ControlButton>
                        <ControlButton boxType={"Cashbox 1"} on:held={openBox}></ControlButton>
                        <ControlButton boxType={"Cashbox 2"} on:held={openBox}></ControlButton>
                        <ControlButton boxType={"Cashbox 3"} on:held={openBox}></ControlButton>
                {:else}
                    Logs
                {/if}
            </div>

        </div>
    </div>

</section>

