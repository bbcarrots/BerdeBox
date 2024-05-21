import { updateMainLockOpen, updateTakePhoto } from '$lib/firebase/database.js';
import { json } from '@sveltejs/kit';

export async function PATCH({ request }) {
	/* Handles Main Lock Open requests. */

	const { boxCode } = await request.json();
	// console.log('PATCH request with boxCode: ' + boxCode);

	let response = await updateMainLockOpen(boxCode)
		.then(() => {
			return { success: true, error: null };
		})
		.catch((error) => {
			return { success: false, error: error };
		});

    response = await updateTakePhoto(boxCode)
		.then(() => {
			return { success: true, error: null };
		})
		.catch((error) => {
			return { success: false, error: error };
		});

	return json(response);
}
