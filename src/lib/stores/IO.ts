import { writable, type Writable } from 'svelte/store';
import { type box } from '$lib/utils/types';

export const Output: Writable<{ [key: string]: boolean | string }> = writable({});
export const InputOutput: Writable<{ [key: string]: boolean | { [key: string]: boolean } }> =
	writable({});
export const Input: Writable<{ [key: string]: boolean }> = writable({});
export const Boxes: Writable<box[]> = writable([]);
