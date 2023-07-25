// See https://kit.svelte.dev/docs/types#app
// for information about these interfaces
declare global {
	namespace App {
		// interface Error {}
		// interface Locals {}
		// interface PageData {}
		// interface Platform {}
		declare module "*.jpg?w=600;1920&format=webp" {
			const image: string[]
			export default image
		}
	}
}



export {};
