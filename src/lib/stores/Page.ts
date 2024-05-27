import { writable, type Writable } from 'svelte/store';

export let loginLoading: Writable<boolean> = writable(false);
export let loading: Writable<boolean> = writable(false);
export let retrievingBoxes: Writable<boolean> = writable(false);
export let loadingProfilePhoto: Writable<boolean> = writable(true);
