import { writable, type Writable } from 'svelte/store';

export let loading: Writable<boolean> = writable(false);