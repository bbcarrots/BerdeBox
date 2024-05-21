<script lang="ts">
    import { firebaseApp } from '$lib/firebase/config'

    import { onMount } from 'svelte'
    import { getMessaging, getToken, onMessage, type Messaging } from 'firebase/messaging'

    let token: string | null = null; //Firebase Cloud Messaging token
    let messages = []; //array of messages received
    let messaging: Messaging;

    onMount(() => {

        //register the service worker before everything else
        if (navigator.serviceWorker) {
            // Register the SW
            navigator.serviceWorker.register('firebase-messaging-sw.js')
            .then(function(registration) {
                console.log('Service Worker registered with scope:', registration.scope);
            }).catch(function(error) {
                console.error('Service Worker registration failed:', error);
            });
        }

        checkPermissions();

        messaging = getMessaging(firebaseApp);
        requestPermission();
        onMessage(messaging, (payload) => {

            messages.push(payload);
            console.log(payload.notification?.title,)
            const notificationOptions = {
                body:  payload.notification?.body
            };

            if (payload.notification?.title !== undefined){
                new Notification(payload.notification?.title, notificationOptions);
            }

        })

    })


    // funciton to request permission for notifications
    function requestPermission(){
        Notification.requestPermission().then(permission => {
            // if the permission has been granted, get the token
            if (permission === 'granted'){

                //get the token and set the fetched token to the user's token
                getToken(messaging, {vapidKey: 'BAgbjDYolVbTrQZZ5y6zyf1Fmt2DnvVeK5fd2_34XM88gKL9W52RS2YwCRSvK3cW1BTnXG1SgTaGHUpJpRkhqdc'}).then(fetchedToken => {
                    token = fetchedToken;
                    console.log(token)
                }).catch(error => {
                    // edit handler for 
                    console.log('Error fetching token', error)
                })
            }

        })
    }

    // shows the notification
    function showNotification(){
        let notificationOptions = {
            body: 'Dummy notif info',
            icon: '<>'
        };

        let notif  = new Notification('Test new notification', notificationOptions);
        console.log(notif)
        notif.onclick = () => {
            console.log('Notification clicked');
        }
    }

    function checkPermissions(){
        if(window.Notification) {
            if(Notification.permission === 'granted') {
            } else if(Notification.permission !== 'denied') {
                Notification.requestPermission(permission => {
                if(permission === 'granted') {
                }
                })
            }
        }
    }

</script>

Settings
<button on:click={showNotification}>
    Show notification
</button>

<button on:click={requestPermission}>
    Enable notifications
</button>