import { firebaseApp } from '$lib/firebase/config';
import type { FirebaseApp } from 'firebase/app';
import { Database, getDatabase } from 'firebase/database';
import { writable, type Writable } from 'svelte/store';

export const firebaseDBFront: Writable<Database> = writable(getDatabase(firebaseApp));
export const firebaseAppFront: Writable<FirebaseApp> = writable(firebaseApp)
