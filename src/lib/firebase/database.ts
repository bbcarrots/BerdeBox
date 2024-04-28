import { firebaseApp } from './config';
import { getDatabase, ref, onValue, set } from 'firebase/database';

export const firebase = getDatabase(firebaseApp);

export function writeLed(is_on: boolean) {
	set(ref(firebase), {
		led_is_on: is_on
	});
}

const reference = ref(firebase);
onValue(reference, (snapshot) => {
  const data = snapshot.val();
  console.log(data)
});