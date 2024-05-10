import { updateLED } from '$lib/firebase/database.js';
import { json } from '@sveltejs/kit';

export async function PATCH({ request }) {
	/* Handles Update requests for usage log records. */
	const { led_is_on } = await request.json();
	return json(updateLED(led_is_on));
}