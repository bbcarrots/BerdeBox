import { firebaseApp } from './config';
import { getStorage, ref, getDownloadURL, listAll, getMetadata } from 'firebase/storage';
import { Logs } from '$lib/stores/IO';

const storage = getStorage(firebaseApp);

//function to get image from firebase and returns the image source
export async function getImage(imageName: string): Promise<string | null> {
	try {
		//path is images/[imageName]
		const storageRef = ref(storage, 'images/' + imageName);
		const url = await getDownloadURL(storageRef);
		return url;
	} catch (error) {
		console.error('Error getting image:', error);
		return null;
	}
}

// this function takes in the action and the status from the filename
// it returns the associated message to be displayed in the frontend.
function get_message(status: string, action: string) {
	console.log(action.includes('mbopen'), status);
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

export async function getImages(mailbox_num: number, date: string) {
	const mailboxPics = ref(storage, `berdebox${mailbox_num}/${date}`);

	try {
		const response = await listAll(mailboxPics);
		const { items } = response;

		// Fetch URLs and metadata for each item
		const urls = await Promise.all(items.map((item) => getDownloadURL(item)));
		const metadataArray = await Promise.all(items.map((item) => getMetadata(item)));

		// Create log entries
		const logs = items.map((item, index) => {
			const metadata = metadataArray[index];

			// information[0] == id number
			// information[1] == status
			// information[2] == action

			const information = metadata.name.split('-');

			console.log(information);
			return {
				id: Number(information[0]),
				imageURL: urls[index],
				datetime: new Date(metadata.timeCreated),
				message: get_message(information[1], information[2]),
				status: information[1]
			};
		});

		// Update the Logs store with the new log entries
		Logs.set(logs);
	} catch (error) {
		console.error('Error retrieving photos or metadata:', error);
	}
}
