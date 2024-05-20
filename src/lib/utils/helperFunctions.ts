import { getImages } from "$lib/firebase/storage";
import { Boxes } from "$lib/stores/IO";

export async function initializeBox(id: number){
	let logs = await getImages(id, '05-19-2024');
	let reversedLogs = [...logs].reverse()

    Boxes.update(currentBoxes => [
        ...currentBoxes, 
        {
            id: id,
            logs: reversedLogs
        }
    ]);
}

// this function takes in the action and the status from the filename
// it returns the associated message to be displayed in the frontend.
export function getMessage(status: string, action: string) {
	console.log(action.includes('mbopen'), status);
	if (action.includes('mbopen')) {
		if (status == 'success') {
			return 'Mailbox successfully opened.';
		} else {
			return 'Mailbox not opened successfully.';
		}
	} else if (action.includes('doorbell')) {
		return 'Mailbox doorbell was rang!';
	} else if (action.includes('cbopen')) {
		let cashboxNum = action.slice(-5).slice(0, -4);
		if (status == 'success') {
			return `Cashbox ${cashboxNum} successfully opened.`;
		} else {
			return `Cashbox ${cashboxNum} not opened successfully.`;
		}
	}
	return '';
}