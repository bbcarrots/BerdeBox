<script lang="ts">
    import { page } from '$app/stores';
    import ControlButton from "$lib/components/ControlButton.svelte";

    const boxID = Number($page.params.boxID);

    function openBox(){
        console.log("action!")
    }

</script>

<section>
    <p>Photo goes here</p>

    <div id='tabbar'>
        <ControlButton boxType={"Mailbox"} on:held={openBox}></ControlButton>
    </div>
</section>

