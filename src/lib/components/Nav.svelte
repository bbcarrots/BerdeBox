<script lang="ts">
    import { page } from "$app/stores";

    $: activeUrl = $page.url.pathname;

    const activeClasses = 'bg-black text-white font-medium rounded-full text-sm px-5 py-2.5 text-center me-2 mb-2'
    const inactiveClasses = 'border border-black font-medium rounded-full text-sm px-5 py-2.5 text-center me-2 mb-2'
</script>

<div>
    <a href="/boxes">
        <button type="button" class="{activeUrl === '/boxes' ? activeClasses : inactiveClasses}">
            Boxes
        </button>
    </a>
    <a href="/modules">
        <button type="button" class="{activeUrl === '/modules' ? activeClasses : inactiveClasses}">
            Modules
        </button>
    </a>
</div>
