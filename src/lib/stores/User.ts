import { writable, type Writable } from 'svelte/store';

export type User = {
	name: string | null;
	profilePhoto: string | null;
	uid: string;
	notifToken: string;
	boxes: number[];
	notifsPermitted: boolean;
};

export const UserStore: Writable<User> = writable({
	name: '',
	profilePhoto: '',
	uid: '',
	notifToken: '',
	boxes: [],
	notifsPermitted: false
});

export const notifsPermitted: Writable<boolean> = writable(false);
