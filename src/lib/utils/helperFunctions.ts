import { getImages } from '$lib/firebase/storage';
import { Boxes } from '$lib/stores/IO';
import { getBerdeBoxes } from '$lib/firebase/firestore';
import { UserStore } from '$lib/stores/User';
import { getBoxByRef } from '$lib/firebase/firestore';

//box to initialize it in the UI
export async function initializeBox(id: number) {
	// check if the box exits
	let berdeBoxes = await getBerdeBoxes();

	berdeBoxes.forEach((box) => {
		//if the box exists
		if (Number(box.id) == id) {
			// add it to the user's firestore
			// load the boxes again to update
		}
		// if the box doesn't exist
		else {
			console.log('Box does not exist');
		}
	});

	let logs = await getImages(id);
	let reversedLogs = [...logs].reverse();

	Boxes.update((currentBoxes) => [
		...currentBoxes,
		{
			id: id,
			logs: reversedLogs
		}
	]);
}

// this function takes in the action and the status from the filename
// it returns the associated message to be displayed in the frontend.
export function getMessage(status: string, action: string) {
	if (action.includes('mbopen')) {
		if (status == 'success') {
			return 'Mailbox successfully opened.';
		} else {
			return 'Mailbox not opened successfully.';
		}
	} else if (action.includes('doorbell')) {
		return 'Mailbox doorbell was rang!';
	} else if (action.includes('cbopen')) {
		let cashboxNum = action.slice(-5).slice(0, -4);
		if (status == 'success') {
			return `Cashbox ${cashboxNum} successfully opened.`;
		} else {
			return `Cashbox ${cashboxNum} not opened successfully.`;
		}
	}
	return '';
}

export function checkPermissions() {
	if (window.Notification) {
		if (Notification.permission === 'granted') {
		} else if (Notification.permission !== 'denied') {
			Notification.requestPermission((permission) => {
				if (permission === 'granted') {
				}
			});
		}
	}
}

export async function loadUserBoxes() {
	$UserStore.boxes.forEach(async (boxRef: any) => {
		// obtain the box
		let box: any = await getBoxByRef(boxRef);
		const logs = await getImages(box.id);

		console.log(box);
		// update the boxes store
		Boxes.update((currentBoxes) => [
			...currentBoxes,
			{
				id: box.id,
				logs: logs
			}
		]);
	});
}
