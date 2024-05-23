export type log = {
	id: number;
	imageURL: string;
	datetime: Date;
	message: string;
	status: string;
};

export type box = {
	id: number;
	logs: log[];
};
