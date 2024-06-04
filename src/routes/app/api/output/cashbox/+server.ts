import { updateCashLockOpen, updateTakePhoto } from '$lib/firebase/database.js';
import { json } from '@sveltejs/kit';

export async function PATCH({ request }) {
	/* Handles Cash Box Open requests. */

	const { boxCode, cashBoxCode } = await request.json();

	let response = await updateCashLockOpen(boxCode, cashBoxCode)
		.then(() => {
			return { success: true, error: null };
		})
		.catch((error) => {
			return { success: false, error: error };
		});

    response = await updateTakePhoto(boxCode, 3)
		.then(() => {
			return { success: true, error: null };
		})
		.catch((error) => {
			return { success: false, error: error };
		});

	return json(response);
}