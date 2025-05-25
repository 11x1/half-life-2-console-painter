#include "interfaces.hh"
#include "utils.hh"

void interfaces::setup( ) {
    START_LIST( logger::prefixes::debug, "getting interfaces", 5, )

    entitylist = utils::create_interface< client_entitylist >( "client.dll", "VClientEntityList003" );
    LOG_LIST( "entitylist = {:p}", reinterpret_cast< void* >( entitylist ) );

    // sig matches too much, find a better place / use getinterface
    const auto mod = utils::get_module( "engine.dll" );

    // all sigs too vague
    const auto engine_render_addr = mod->get_offset_address( 0x14EAC9 + 0x2 );
    LOG_LIST( "engine_render_addr = {:p}", reinterpret_cast< void* >( engine_render_addr ) );

    // so initially thought this shouldve been a single deref
    // but turns out its a double deref
    // going to the single deref in memory view in cheat engine
    // got me to an object, derefing that object again
    // in theory wouldve pointed me to a vtbl (wrong, no clue why),
    // tried to deref again, boom correct vtbl -> double deref for obj
    engine_renderer = **reinterpret_cast< crender *** >( engine_render_addr );
    LOG_LIST( "engine_renderer off=0x{:x} ptr={:p}", mod->get_offset( engine_render_addr ), reinterpret_cast< void* >( interfaces::engine_renderer ) );

    const auto csurf_addr = utils::scan_pattern( "client.dll", "8B 0D ? ? ? ? 52 8B 01 FF 50 ? E9" ) + 2;
    c_surface = **reinterpret_cast< surface *** >( csurf_addr );
    LOG_LIST( "c_surface off=0x{:x} ptr={:p}", utils::get_module( "client.dll" )->get_offset( csurf_addr ), reinterpret_cast< void* >( interfaces::c_surface ) );

    // this sets the last used list to a nullptr, so nothing hereon out is logged
    // fix: components get height, a list should be embeddable in a list
    //      (list max_lines -> max components)
    //      and make the macros use a stack-based way of keeping track
    //      of the last list
    vgui_panel = utils::bruteforce_interface< cpanel >( "VGUI_Panel009" );
    LOG_LIST( "vgui_panel ptr={:p}", reinterpret_cast< void* >( interfaces::vgui_panel ) );

    const auto memalloc_addr = utils::scan_pattern( "tier0.dll", "8B 0D ? ? ? ? FF 75 ? 8B 01 FF 50 ? 68 ? ? ? ? 8B F8" );
    // import, aka (i think) object "lives" in
    // the imported section of the module
    // so basically a obj** inside gameui...?
    //
    // single deref's deref got me into
    // tier0's address space but for the global
    // memalloc object
    LOG( error, "memalloc_pat_offset=0x{:x}", utils::get_module( "GameUI.dll" )->get_offset( memalloc_addr ) );
    LOG( error, "memalloc_global_offset=0x{:x}", utils::get_module( "GameUI.dll" )->get_offset( *reinterpret_cast< uintptr_t* >( memalloc_addr + 2 ) ) )
    LOG( error, "memalloc_ptr={:p}", **reinterpret_cast< void*** >( memalloc_addr + 2 ) )
    LOG( error, "memalloc_vtbl={:p}", ***reinterpret_cast< void**** >( memalloc_addr + 2 ) )
    memalloc = **reinterpret_cast< ::memalloc *** >( memalloc_addr + 2 );
    LOG_LIST( "memalloc off=0x{:x} ptr={:p}", utils::get_module( "GameUI.dll" )->get_offset( memalloc_addr ), reinterpret_cast< void* >( interfaces::memalloc ) );

    // get interface def
    using create_or_find_interface = int( __cdecl* )( int, const char* );

    // find the exported func from steam_api module
    const auto SteamInternal_FindOrCreateUserInterface = utils::get_proc_address< create_or_find_interface >(
        "steam_api.dll", "SteamInternal_FindOrCreateUserInterface" );

    // if we found it
    if ( SteamInternal_FindOrCreateUserInterface ) {
        steamutils = reinterpret_cast< ::steamutils * >( SteamInternal_FindOrCreateUserInterface( 0, "SteamUtils010" ) );
    }

    END_LIST( logger::prefixes::success, "done" );
}
