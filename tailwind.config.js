/** @type {import('tailwindcss').Config} */
export default {
	content: ['./src/**/*.{html,js,svelte,ts}'],
	theme: {
		extend: {
			colors: {
				'bb-black': '#303030',
				'bb-green-1': '#d4edd1',
				'bb-green-2': '#304336',
				'bb-green-3': '#1b3022',
				'bb-green-4': '#95ba90',
				'bb-black': '#303030',
				'bb-light-green': '#d4edd1',
				'bb-green': '#95ba90',
				'bb-dark-green': '#304336',
				'bb-darkest-green': '#1b3022',
			}
		}
	},
	plugins: []
};
