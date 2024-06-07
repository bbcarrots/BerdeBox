import 'firebaseui/dist/firebaseui.css';
import { firebaseApp } from './config';
import { getAuth, GoogleAuthProvider, signInWithPopup, signOut } from 'firebase/auth';
import { goto } from '$app/navigation';

export const auth = getAuth(firebaseApp);
const provider = new GoogleAuthProvider();

// Function to trigger login
export const login = () => {
	signInWithPopup(auth, provider)
		.then((result) => {
			// This gives you a Google Access Token. You can use it to access Google APIs.
			let credential = null;

			if (result) {
				credential = GoogleAuthProvider.credentialFromResult(result);
				const user = result.user;
			}
			const token = credential?.accessToken;
			const user = result?.user;
		})
		.catch((error) => {
			const errorCode = error.code;
			const errorMessage = error.message;
			const email = error.customData.email;
			const credential = GoogleAuthProvider.credentialFromError(error);
			console.log(errorCode, errorMessage, email, credential);
		});
};

export function handleSignOut() {
	signOut(auth)
		.then(() => {
			console.log('Sign out successful');
			// Sign-out successful.
			goto('/app');
		})
		.catch((error) => {
			console.log('Sign out error', error);
			// An error happened.
		});
}
