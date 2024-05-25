import { writable, type Writable } from 'svelte/store';

export let loading: Writable<boolean> = writable(false);
export let retrievingBoxes: Writable<boolean> = writable(false);