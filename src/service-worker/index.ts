/// <reference types="@sveltejs/kit" />
/// <reference lib="webworker" />

declare let self: ServiceWorkerGlobalScope

// @ts-nocheck
import { build, files, version } from '$service-worker';

// Create a unique cache name for this deployment
const CACHE = `cache-${version}`;

const ASSETS = [
    ...build, // the app itself
    ...files  // everything in `static`
];

self.addEventListener('install', (event) => {
    // Create a new cache and add all files to it
    async function addFilesToCache() {
        const cache = await caches.open(CACHE);
        await cache.addAll(ASSETS);
    }

    event.waitUntil(addFilesToCache());
});

self.addEventListener('activate', (event) => {
    // Remove previous cached data from disk
    async function deleteOldCaches() {
        for (const key of await caches.keys()) {
            if (key !== CACHE) await caches.delete(key);
        }
    }

    event.waitUntil(deleteOldCaches());
});

self.addEventListener('fetch', (event) => {
    // ignore POST requests etc
    if (event.request.method !== 'GET') return;

    async function respond() {
        const url = new URL(event.request.url);
        const cache = await caches.open(CACHE);

        // `build`/`files` can always be served from the cache
        if (ASSETS.includes(url.pathname)) {
            return cache.match(url.pathname);
        }

        // for everything else, try the network first, but
        // fall back to the cache if we're offline
        try {
            const response = await fetch(event.request);
            const isNotExtension = url.protocol === 'http:'
            const isSuccess = response.status === 200

            if (isSuccess && isNotExtension) {
                cache.put(event.request, response.clone());
            }

            return response;
        } catch {
            const cachedRespose = await cache.match(url.pathname)
            if (cachedRespose) {
                return cachedRespose;
            }
        }

        return new Response('Not Found', {status: 404})
    }

    event.respondWith(respond().then(response => {
        if (response) {
          return response; // Return the valid response
        } else {
          // Handle the case where response is undefined
          // For example, return a custom error response or fetch another resource
          return new Response("Resource not found", { status: 404 });
        }
      }));
});

self.addEventListener('message', event => {
    if (event.data && event.data.type === 'SKIP_WAITING'){
        self.skipWaiting();
    }
})