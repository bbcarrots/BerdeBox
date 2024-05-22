import { firebaseApp } from './config';
import { getFirestore, doc, getDoc, setDoc } from 'firebase/firestore';
import { collection, query, getDocs } from 'firebase/firestore';

const firestoreDB = getFirestore(firebaseApp);
const berdeBoxesQuery = query(collection(firestoreDB, 'berdeboxes'));
const usersQuery = query(collection(firestoreDB, 'users'));

export async function getBerdeBoxes() {
	const querySnapshot = await getDocs(berdeBoxesQuery);
	querySnapshot.forEach((doc) => {
		// doc.data() is never undefined for query doc snapshots
		console.log(doc.id, ' => ', doc.data());
	});
	return querySnapshot;
}

export async function getUsers() {
	const querySnapshot = await getDocs(usersQuery);
	querySnapshot.forEach((doc) => {
		// doc.data() is never undefined for query doc snapshots
		console.log(doc.id, ' => ', doc.data());
	});
}

export async function getUserbyID(docID: string) {
	const usersRef = doc(firestoreDB, 'users', docID);
	const querySnapshot = await getDoc(usersRef);

	if (querySnapshot.exists()) {
		return querySnapshot.data(); // Return the data of the document
	} else {
		return null;
	}
}

export async function getBoxbyID(docID: string) {
	const berdeBoxesRef = doc(firestoreDB, 'berdeboxes', docID);
	const querySnapshot = await getDoc(berdeBoxesRef);

	if (querySnapshot.exists()) {
		return true;
	} else {
		return false;
	}
}

export async function getBoxByRef(ref: any) {
	const querySnapshot = await getDoc(ref);

	if (querySnapshot.exists()) {
		return querySnapshot.data();
	} else {
		return null;
	}
}

export async function addUser(docID: string) {
	await setDoc(doc(firestoreDB, 'users', docID), {
		notifToken: '',
		berdeboxes: []
	});
}

export async function updateUserToken(docID: string, notifToken: string) {
	const usersRef = doc(firestoreDB, 'users', docID);
	const querySnapshot = await getDoc(usersRef);

	if (querySnapshot.exists()) {
		setDoc(usersRef, {
			berdeboxes: querySnapshot.data().berdeboxes,
			notifToken: notifToken
		});

		return true;
	} else {
		return false;
	}
}

export async function updateUserBoxes(docID: string, box: any) {
	const usersRef = doc(firestoreDB, 'users', docID);
	const querySnapshot = await getDoc(usersRef);

	console.log(box);
	if (querySnapshot.exists()) {
		setDoc(usersRef, {
			berdeboxes: [...querySnapshot.data().berdeboxes, box],
			notifToken: querySnapshot.data().notifToken
		});
		return true;
	} else {
		return false;
	}
}
