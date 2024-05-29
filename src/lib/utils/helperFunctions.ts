// this function takes in the action and the status from the filename
// it returns the associated message to be displayed in the frontend.
export function getMessage(status: string, action: string) {
	if (action.includes('mbopen')) {
		if (status == 'success') {
			return 'Mailbox successfully opened.';
		} else {
			return 'Mailbox not opened successfully.';
		}
	} else if (action.includes('doorbell')) {
		return 'Mailbox doorbell was rang!';
	} else {
		let match = action.match(/cb(\d+)open/);
		let cashboxNum = match ? match[1] : null;

		if (cashboxNum) {
			if (status == 'success') {
				return `Cashbox ${cashboxNum} successfully opened.`;
			} else {
				return `Cashbox ${cashboxNum} not opened successfully.`;
			}
		} else {
			return `Invalid action format: ${action}`;
		}
	}
	return '';
}

export function checkPermissions() {
	if (window.Notification) {
		if (Notification.permission === 'granted') {
		} else if (Notification.permission !== 'denied') {
			Notification.requestPermission((permission) => {
				if (permission === 'granted') {
				}
			});
		}
	}
}
