import { updateMainLockOn } from '$lib/firebase/database.js';
import { json } from '@sveltejs/kit';

export async function PATCH({ request }) {
    /* Handles Open Main Lock requests. */

    const { boxCode } = await request.json();
    console.log("PATCH request with boxCode: " + boxCode)

    const response = await updateMainLockOn(boxCode).then(() => { return {success: true, error: null} }).catch((error) => {return {success: false, error: error}})

    return json(response)
}