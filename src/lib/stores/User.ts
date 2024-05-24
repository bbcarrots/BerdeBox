import { writable, type Writable } from 'svelte/store';

export type User = {
	uid: string;
	notifToken: string;
	boxes: number[];
	notifsPermitted: boolean;
};

export const UserStore: Writable<User> = writable({
	uid: '',
	notifToken: '',
	boxes: [],
	notifsPermitted: false
});

export const notifsPermitted: Writable<boolean> = writable(false);