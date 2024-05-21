import { firebaseApp } from './config';
import { getStorage, ref, getDownloadURL, listAll, getMetadata } from 'firebase/storage';
import { getMessage } from '$lib/utils/helperFunctions';

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

			// console.log(information);
			return {
				id: Number(information[0]),
				imageURL: urls[index],
				datetime: new Date(metadata.timeCreated),
				message: getMessage(information[1], information[2]),
				status: information[1]
			};
		});

		// Update the Logs store with the new log entries
		return logs
	} catch (error) {
		console.error('Error retrieving photos or metadata:', error);
		return []
	}
}
