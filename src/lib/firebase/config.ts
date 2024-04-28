import { initializeApp } from 'firebase/app';

const firebaseConfig = {
    apiKey: "AIzaSyCTTM_Z9FNavPy8Phs5Z_VqOlmneMZcAFM",
    authDomain: "cs145-0x0-b0x0-labs-berdebox.firebaseapp.com",
    databaseURL: "https://cs145-0x0-b0x0-labs-berdebox-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "cs145-0x0-b0x0-labs-berdebox",
    storageBucket: "cs145-0x0-b0x0-labs-berdebox.appspot.com",
    messagingSenderId: "26502398185",
    appId: "1:26502398185:web:95512c0a55f89b10f2f078"
  };

export const firebaseApp = initializeApp(firebaseConfig);
