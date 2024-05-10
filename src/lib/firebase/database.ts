import { firebaseApp } from './config';
import { getDatabase, ref, update } from 'firebase/database';

const firebase = getDatabase(firebaseApp);

export const outputRef = ref(firebase, 'bb1/output')
// export const inputOutputRef = ref(firebase, 'input_output')
// export const inputRef = ref(firebase, 'input')

export function updateLED(isOn: boolean) {
    /* Updates the value of `output.led_is_on` in real-time DB. */
    const updateValue = {
		led_is_on: isOn
	}

	update(outputRef, updateValue);

    return {
        success : true
    }
}

// export function updateSpeaker(audio: string) {
//     /* Updates the value of `output.speaker_play` in real-time DB. */
//     const updateValue = {
// 		speaker_play: audio
// 	}

// 	update(outputRef, updateValue);
// }

// export function updateMainLock(isOn: boolean) {
//     /* Updates the value of `input_output.main_lock_is_on` in real-time DB. */
//     const updateValue = {
// 		main_lock_is_on: isOn
// 	}

// 	update(inputOutputRef, updateValue);
// }

// export function updateCashLock(isOn: boolean, boxNumber: number) {
//     /* Updates the value of `input_output.${boxNumber}_is_on` in real-time DB. */
//     const property: string = `${boxNumber}_is_on`
//     const updateValue: {[key: string]: boolean} = {}
//     updateValue[property] = isOn

// 	update(inputOutputRef, updateValue);
// }

// export function updateButton(isPressed: boolean) {
//     /* Updates the value of `input_output.button_is_pressed` in real-time DB. */
//     const updateValue = {
// 		button_is_pressed: isPressed
// 	}

// 	update(inputRef, updateValue);
// }

// export function updateCamera(isOn: boolean) {
//     /* Updates the value of `input_output.camera_is_on` in real-time DB. */
//     const updateValue = {
// 		camera_is_on: isOn
// 	}

// 	update(inputRef, updateValue);
// }