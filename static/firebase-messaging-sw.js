importScripts("https://www.gstatic.com/firebasejs/8.0.0/firebase-app.js");
importScripts("https://www.gstatic.com/firebasejs/8.0.0/firebase-messaging.js");

const firebaseConfig = {
  apiKey: "AIzaSyCTTM_Z9FNavPy8Phs5Z_VqOlmneMZcAFM",
  authDomain: "cs145-0x0-b0x0-labs-berdebox.firebaseapp.com",
  databaseURL: "https://cs145-0x0-b0x0-labs-berdebox-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "cs145-0x0-b0x0-labs-berdebox",
  storageBucket: "cs145-0x0-b0x0-labs-berdebox.appspot.com",
  messagingSenderId: "26502398185",
  appId: "1:26502398185:web:95512c0a55f89b10f2f078"
};

// Retrieve an instance of Firebase Messaging so that it can handle background
// messages.
const firebaseApp = firebase.initializeApp(firebaseConfig)
const messaging = firebase.messaging(firebaseApp)
messaging.onBackgroundMessage(messaging, (payload) => {
  console.log('[firebase-messaging-sw.js] Received background message ', payload);
  // Customize notification here
  const notificationTitle = payload.notification.title
  const notificationOptions = {
    body: payload.notification.body,
    icon: payload.notification.icon
  }

  self.registration.showNotification(notificationTitle,
    notificationOptions)
})