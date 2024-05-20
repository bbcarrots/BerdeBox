import { updateCashLockOpen } from '$lib/firebase/database.js';
import { json } from '@sveltejs/kit';

export async function PATCH({ request }) {
	/* Handles Cash Box Open requests. */

	const { boxCode, cashBoxCode } = await request.json();

	const response = await updateCashLockOpen(boxCode, cashBoxCode)
		.then(() => {
			return { success: true, error: null };
		})
		.catch((error) => {
			return { success: false, error: error };
		});

	return json(response);
}
