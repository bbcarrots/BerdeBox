import { Boxes } from '$lib/stores/IO';
import { getBoxByRef } from '$lib/firebase/firestore';
import { getImages } from '$lib/firebase/storage';

// This function updates the Boxes Store given box references
export async function updateBoxesStore(boxRefs: any) {
	let boxes: any[] = [];

	// Load user boxes and save to a local array
	for (const boxRef of boxRefs) {
		// Obtain the box, get the images of each box
		// Store the logs in reverse chronological order
		let box: any = await getBoxByRef(boxRef);
		const logs = await getImages(box.id);
		let reversedLogs = [...logs].reverse();
		boxes = [
			...boxes,
			{
				id: box.id,
				logs: reversedLogs
			}
		];
	}

	// Update the boxes store
	Boxes.set(boxes);
	return true;
}

// This function checks if the box, given the id, is already in the user's boxes
// This will be used to prevent duplicates when adding the same box with the same ID
export async function isBoxInUserBoxes(boxID: number, userBoxes: any[]): Promise<boolean> {
	for (const boxRef of userBoxes) {
		const userBox: any = await getBoxByRef(boxRef);
		if (Number(userBox.id) === boxID) {
			return true;
		}
	}
	return false;
}
