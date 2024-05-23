import { loading } from '$lib/stores/Page.js';

export function load() {
    loading.set(false);
}