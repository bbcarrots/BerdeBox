import { writable, type Writable } from 'svelte/store';

export type User = {
	uid: string;
	notifToken: string;
	boxes: number[];
};

export const UserStore: Writable<User> = writable({
	uid: '',
	notifToken: '',
	boxes: []
});
