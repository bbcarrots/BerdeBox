import { firebaseApp } from './config';
import { getDatabase, ref, update } from 'firebase/database';

const firebase = getDatabase(firebaseApp);

export const outputRef = ref(firebase, 'output')
export const inputOutputRef = ref(firebase, 'input_output')
export const inputRef = ref(firebase, 'input')

export function updateMainLockOpen(boxCode: string) {
    /* Updates the values of `main_lock_is_open`, `lock_buzzer_is_on`, `lock_led_is_on`, and `take_photo`. */
    const updateValue = {
		main_lock_is_open: true,
        lock_buzzer_is_on: true,
        lock_led_is_on: true,
        take_photo: true
	}

	return update(ref(firebase, boxCode + '/output'), updateValue)
}

export function updateCashLockOpen(boxCode: string, cashBoxCode: string) {
    /* Updates the value of `cb<#>_is_open` depending on the cashBoxID */
    const updateValue: {[key: string]: boolean} = {}
    updateValue[cashBoxCode + "_is_open"] = true

	return update(ref(firebase, boxCode + '/output/modules'), updateValue);
}

export function updateButton(isPressed: boolean) {
    /* Updates the value of `input_output.button_is_pressed` in real-time DB. */
    const updateValue = {
		button_is_pressed: isPressed
	}

	update(inputRef, updateValue);
}

export function updateCamera(isOn: boolean) {
    /* Updates the value of `input_output.camera_is_on` in real-time DB. */
    const updateValue = {
		camera_is_on: isOn
	}

	update(inputRef, updateValue);
}

export function updateLED(isOn: boolean) {
    /* Updates the value of `output.led_is_on` in real-time DB. */
    const updateValue = {
		led_is_on: isOn
	}

	update(outputRef, updateValue);
}

export function updateSpeaker(audio: string) {
    /* Updates the value of `output.speaker_play` in real-time DB. */
    const updateValue = {
		speaker_play: audio
	}

	update(outputRef, updateValue);
}