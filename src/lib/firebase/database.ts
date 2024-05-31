import { firebaseApp } from './config';
import { getDatabase, ref, update } from 'firebase/database';

const firebase = getDatabase(firebaseApp);

export function updateMainLockOpen(boxCode: string) {
	/* Updates the values of `main_lock_is_open`, `lock_buzzer_is_on`, `lock_led_is_on`, and `take_photo`. */
	const updateValue: { [key: string]: boolean } = { main_lock_is_open: true };

	return update(ref(firebase, boxCode + '/output'), updateValue);
}

export function updateCashLockOpen(boxCode: string, cashBoxCode: string) {
	/* Updates the value of `cb<#>_is_open` depending on the cashBoxID */
	const updateValue: { [key: string]: boolean } = {};
	updateValue[cashBoxCode + '_is_open'] = true;

	return update(ref(firebase, boxCode + '/output/modules'), updateValue);
}

export function updateTakePhoto(boxCode: string, command: number) {
	/* Updates the value of `cb<#>_is_open` depending on the cashBoxID */
	const updateValue: { [key: string]: boolean | number } = {
		take_photo: true,
        take_photo_command: command
	};

	return update(ref(firebase, boxCode + '/output'), updateValue);
}
