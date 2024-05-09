<script>
	import { onMount } from 'svelte';
	import '../app.css';
	import { getImage } from "$lib/firebase/storage";
	import { ImageURLs } from '$lib/stores/IO';

	// async function detectSWUpdate(){
	// 	const registration = await navigator.serviceWorker.ready;

	// 	registration.addEventListener('updatefound', () =>{
	// 		const newSW = registration.installing;
	// 		newSW?.addEventListener('statechange', ()=>{
	// 			if (newSW.state === 'installed') {
	// 				if (confirm('New update available! Reload to update?')){
	// 					newSW.postMessage({type: 'SKIP_WAITING'})
	// 					window.location.reload();
	// 				}
	// 			}
	// 		})
	// 	})
	// }

	// onMount(() => {
	// 	detectSWUpdate();
	// })

	
    //todo: update in the future to get all of the images in a folder
    onMount(() => {
        getImage('delivery_man.jpeg').then(url => {
            if (url !== null) { 
                const currentURLs = $ImageURLs;

                const updatedURLs = [...currentURLs, url];

                ImageURLs.set(updatedURLs);
            }
        });
    });
</script>

<body class="bg-[#EEF2F5]">
	<slot />
</body>

<style>
	@import url('https://fonts.googleapis.com/css2?family=Familjen+Grotesk:ital,wght@0,400..700;1,400..700&display=swap');

	:global(body){
		font-family: "Familjen Grotesk", sans-serif;
	}
</style>