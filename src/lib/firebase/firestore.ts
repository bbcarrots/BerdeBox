import { notifsPermitted } from '$lib/stores/User';
import { firebaseApp } from './config';
import { getFirestore, doc, getDoc, setDoc } from 'firebase/firestore';
import { collection, query, getDocs } from 'firebase/firestore';

const firestoreDB = getFirestore(firebaseApp);
const berdeBoxesQuery = query(collection(firestoreDB, 'berdeboxes'));
const usersQuery = query(collection(firestoreDB, 'users'));

// BERDEBOX OPERATIONS
export async function getBerdeBoxes() {
	const querySnapshot = await getDocs(berdeBoxesQuery);
	return querySnapshot;
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

// USER OPERATIONS
export async function getUsers() {
	const querySnapshot = await getDocs(usersQuery);
	return querySnapshot;
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

export async function addUser(docID: string) {
	await setDoc(doc(firestoreDB, 'users', docID), {
		notifToken: '',
		berdeboxes: [],
		notifsPermitted: false
	});
}

export async function updateUserToken(docID: string, notifToken: string) {
    const usersRef = doc(firestoreDB, 'users', docID);

    try {
        const querySnapshot = await getDoc(usersRef);

        if (querySnapshot.exists()) {
            const userData = querySnapshot.data();
            if (userData && userData.berdeboxes) {

				if (notifToken == '') {
					await setDoc(usersRef, {
						berdeboxes: userData.berdeboxes,
						notifToken: userData.notifToken,
						notifsPermitted: false
					});
				} else {
					await setDoc(usersRef, {
						berdeboxes: userData.berdeboxes,
						notifToken: notifToken,
						notifsPermitted: true
					});
				}
               
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } catch (error) {
        return false;
    }
}

export async function updateUserBoxes(docID: string, box: any) {
	const usersRef = doc(firestoreDB, 'users', docID);
	const querySnapshot = await getDoc(usersRef);

	if (querySnapshot.exists()) {
		setDoc(usersRef, {
			berdeboxes: [...querySnapshot.data().berdeboxes, box],
			notifsPermitted: querySnapshot.data().notifsPermitted,
			notifToken: querySnapshot.data().notifToken
		});
		
		return true;
	} else {
		return false;
	}
}

