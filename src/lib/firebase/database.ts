import { firebaseApp } from './config';
import { getDatabase, ref, update } from 'firebase/database';

const firebase = getDatabase(firebaseApp);

export function updateMainLockOpen(boxCode: string) {
	/* Updates the values of `main_lock_is_open`, `lock_buzzer_is_on`, `lock_led_is_on`, and `take_photo`. */
	const updateValue = {
		main_lock_is_open: true,
		lock_buzzer_is_on: true,
		lock_led_is_on: true,
		take_photo: true
	};

	return update(ref(firebase, boxCode + '/output'), updateValue);
}

export function updateCashLockOpen(boxCode: string, cashBoxCode: string) {
	/* Updates the value of `cb<#>_is_open` depending on the cashBoxID */
	const updateValue: { [key: string]: boolean } = {};
	updateValue[cashBoxCode + '_is_open'] = true;

	return update(ref(firebase, boxCode + '/output/modules'), updateValue);
}

export function updateTakePhoto(boxCode: string) {
	/* Updates the value of `cb<#>_is_open` depending on the cashBoxID */
	const updateValue: { [key: string]: boolean } = { take_photo: true };

	return update(ref(firebase, boxCode + '/output'), updateValue);
}
