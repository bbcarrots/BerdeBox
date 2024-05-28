import firebase from 'firebase/compat/app';
import 'firebaseui/dist/firebaseui.css';
import { firebaseApp } from './config';
import { getAuth, getRedirectResult, GoogleAuthProvider, signInWithPopup, signOut } from 'firebase/auth';
import { goto } from '$app/navigation';

export const auth = getAuth(firebaseApp);

// instance of the google auth provider
const provider = new GoogleAuthProvider();

// optional scope to request for the auth provider
provider.addScope('https://www.googleapis.com/auth/contacts.readonly');

// update language code on Auth instance to localize provider's OAuth flow
auth.languageCode = 'it';

//redirect on auth
signInWithPopup(auth, provider)
	.then((result) => {
		// This gives you a Google Access Token. You can use it to access Google APIs.
		let credential = null; // Initialize credential as null

		if (result) {
			credential = GoogleAuthProvider.credentialFromResult(result);
			const user = result.user;
			console.log(user);
		}
		const token = credential?.accessToken;

		// The signed-in user info.
		const user = result?.user;
		// IdP data available using getAdditionalUserInfo(result)
		// ...
	})
	.catch((error) => {
		// Handle Errors here.
		const errorCode = error.code;
		const errorMessage = error.message;
		// The email of the user's account used.
		const email = error.customData.email;
		// The AuthCredential type that was used.
		const credential = GoogleAuthProvider.credentialFromError(error);
		// ...
	});

//needs to be in a function since firebaseui needs to be dynamically imported
export async function getUIConfig() {
	return {
		signInSuccessUrl: '/',
		signInOptions: [firebase.auth.GoogleAuthProvider.PROVIDER_ID],
		tosUrl: '/',
		privacyPolicyUrl: function () {
			window.location.assign('/');
		}
	};
}

//initialize the firebase auth ui
export async function initializeFirebaseUI(): Promise<void> {
	if (typeof window !== 'undefined') {
		const firebaseui = await import('firebaseui');
		const uiConfig = await getUIConfig();

		var ui = new firebaseui.auth.AuthUI(auth);
		ui.start('#firebaseui-auth-container', uiConfig);
	}
}

export function handleSignOut() {
	signOut(auth)
		.then(() => {
			console.log('Sign out successful');
			// Sign-out successful.
			goto('/');
		})
		.catch((error) => {
			console.log('Sign out error', error);
			// An error happened.
		});
}

export function initApp() {
	console.log('app init');
	firebase.auth().onAuthStateChanged(function (user) {
		console.log('auth change');
		if (user) {
			// User is signed in.
			console.log('user', user);
			var displayName = user.displayName;
			var email = user.email;
			var emailVerified = user.emailVerified;
			var photoURL = user.photoURL;
			var uid = user.uid;
			var phoneNumber = user.phoneNumber;
			var providerData = user.providerData;

			user.getIdToken().then(
				function (accessToken) {
					console.log(accessToken);
				},
				function (error) {
					console.log(error);
				}
			);
		}
	});
}
