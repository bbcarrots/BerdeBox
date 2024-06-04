import { loginLoading } from '$lib/stores/Page.js';

export function load() {
	loginLoading.set(false);
}
