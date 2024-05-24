<script lang="ts">
	import { firebaseApp } from '$lib/firebase/config';
	import { onMount } from 'svelte';
	import { deleteToken, getMessaging, getToken, onMessage, type Messaging } from 'firebase/messaging';
	import { handleSignOut } from '$lib/firebase/auth';
	import { UserStore } from '$lib/stores/User';
	import { updateUserToken } from '$lib/firebase/firestore';
	import { ChevronLeft, Icon, Bell, UserGroup, ChevronRight, ArrowLeftStartOnRectangle } from 'svelte-hero-icons';
	import { goto } from '$app/navigation';
	import { notifsPermitted } from '$lib/stores/User';

	let messages = []; //array of messages received
	let messaging: any;

	onMount(() => {
		//register the service worker before everything else
		if (navigator.serviceWorker) {
			// Register the SW
			navigator.serviceWorker
				.register('firebase-messaging-sw.js')
				.then(function (registration) {
					console.log('Service Worker registered with scope:', registration.scope);
				})
				.catch(function (error) {
					console.error('Service Worker registration failed:', error);
				});
		}

		checkPermissions();

		messaging = getMessaging(firebaseApp);
		requestPermission();
		onMessage(messaging, (payload) => {
			messages.push(payload);
			console.log(payload.notification?.title);
			const notificationOptions = {
				body: payload.notification?.body
			};

			if (payload.notification?.title !== undefined) {
				new Notification(payload.notification?.title, notificationOptions);
			}
		});
	});

	// funciton to request permission for notifications
	function requestPermission() {
		Notification.requestPermission().then((permission) => {
			// if the permission has been granted, get the token
			if (permission === 'granted') {
				//get the token and set the fetched token to the user's token
				getToken(messaging, {
					vapidKey:
						'BAgbjDYolVbTrQZZ5y6zyf1Fmt2DnvVeK5fd2_34XM88gKL9W52RS2YwCRSvK3cW1BTnXG1SgTaGHUpJpRkhqdc'
				})
					.then((fetchedToken) => {
						//update the store
						UserStore.set({
							uid: $UserStore.uid,
							notifToken: fetchedToken,
							boxes: $UserStore.boxes
						});

						//update firestore user information with token
						updateUserToken($UserStore.uid, fetchedToken);
						subscribeTokenToTopic(fetchedToken, 'doorbell-alerts');
						
					})
					.catch((error) => {
						// edit handler for
						console.log('Error fetching token', error);
					});
			}
		});
	}

	function checkPermissions() {
		if (window.Notification) {
			if (Notification.permission === 'granted') {
			} else if (Notification.permission !== 'denied') {
				Notification.requestPermission((permission) => {
					if (permission === 'granted') {
						notifsPermitted.set(true);
					}
				});
			}
		}
	}

	async function subscribeTokenToTopic(token:string, topic: string) {
		const payload = { registrationToken: token, topic: topic}

		const response = await fetch('../../api/topics', {
			method: 'PATCH',
			body: JSON.stringify(payload),
			headers: {
				'content-type': 'application/json'
			}
		})

		console.log(await response.json());
	}

	async function unsubscribeTokenToTopic(token: string, topic: string){

	}

	async function handleNotifToggle(){

		// if the notifs were permitted,
		if ($notifsPermitted){

			// unsubscribe to the topic
			await unsubscribeTokenToTopic($UserStore.notifToken, 'doorbell-alerts');

			// set the permissions back to false
			notifsPermitted.set(false);

			// delete the token
			deleteToken(messaging)
		}

		// if the notifs were not permitted
		else {

			// get the token
			getToken(messaging, {
					vapidKey:
						'BAgbjDYolVbTrQZZ5y6zyf1Fmt2DnvVeK5fd2_34XM88gKL9W52RS2YwCRSvK3cW1BTnXG1SgTaGHUpJpRkhqdc'
				})
					.then((fetchedToken) => {
						//update the store
						UserStore.set({
							uid: $UserStore.uid,
							notifToken: fetchedToken,
							boxes: $UserStore.boxes
						});

						//update firestore user information with token
						updateUserToken($UserStore.uid, fetchedToken);
						subscribeTokenToTopic(fetchedToken, 'doorbell-alerts');
						
					})
					.catch((error) => {
						// edit handler for
						console.log('Error fetching token', error);
					});
				
			}

			notifsPermitted.set(true);

		}
</script>

<section class="h-calc([100%-20px]) max-h-screen">
		<div class="z-20 grid grid-cols-1 w-full h-screen bg-[#EEF2F5] absolute">
			<div class="flex flex-col items-center">
				<!-- Header -->
				<div class="flex items-center justify-between p-4 w-full my-4">
					<button
						on:click={() => {goto('/boxes')}}
						class="right-2"
					>
						<Icon src={ChevronLeft} solid size="20" />
					</button>
					<h4>Settings</h4>
					<div></div>
				</div>

				<!-- SETTING ITEMS -->
				<div class="w-[95%]">
					<p class="w-full p-2 my-2">General</p>

					<!-- Main settings group -->
					<div class="grid bg-white w-full p-1 gap-1 rounded-[15px] border border-[#D9D9D9]/[.5]">
						
						<!-- Enable notification toggle -->
						<button class="flex flex-row justify-between items-center p-5" on:click={requestPermission}> 
	
							<div class="flex flex-row itens-center gap-3">
								<span class="text-bb-black/[0.8]">
									<Icon src={Bell} outline size="25" />
								</span>
	
								<p>Enable notifications</p> 
							</div>
							
							<label class="inline-flex items-center cursor-pointer">
								<input type="checkbox" class="sr-only peer" bind:checked={$notifsPermitted}>
								<div class="relative w-11 h-6 bg-gray-200 peer-focus:outline-none rounded-full peer peer-checked:after:translate-x-full peer-checked:after:border-white after:content-[''] after:absolute after:top-[2px] after:start-[2px] after:bg-white after:border-gray-300 after:border after:rounded-full after:h-5 after:w-5 after:transition-all peer-checked:bg-green-600"></div>
							</label>
						</button>
	
						<!-- Divider -->
						<div class="w-[95%] h-px bg-[#D9D9D9]/[.5] mx-[2.5%]"></div>

						<!-- About redirect -->
						<button class="flex flex-row justify-between items-center p-5" on:click> 
							<div class="flex flex-row itens-center gap-3">
								<span class="text-bb-black/[0.8]">
									<Icon src={UserGroup} outline size="25" />
								</span>
	
								<p>About b0x0 Labs</p> 
							</div>

							<span class="text-bb-black/[0.7]">
								<Icon src={ChevronRight} outline micro size="25" />
							</span>

						</button>
			
					</div>
		
					<button class="flex flex-row justify-between items-center p-5" on:click={handleSignOut}> 
						<div class="flex flex-row itens-center gap-3">

							<span class="text-bb-black/[0.8]">
								<Icon src={ArrowLeftStartOnRectangle} outline size="25" />
							</span>

							<p>Sign Out</p> 
						</div>
					
					</button>
		
					</div>
				</div>
			</div>
</section>
