#include "interfaces.hh"
#include "utils.hh"

void interfaces::setup( ) {
    entitylist = utils::create_interface< client_entitylist >( "client.dll", "VClientEntityList003" );

    // sig matches too much, find a better place / use getinterface
    const auto mod = utils::get_module( "engine.dll" );

    // all sigs too vague
    const auto engine_render_addr = mod->get_offset_address( 0x14EAC9 + 0x2 );
    printf( "enginerender addr=%p\n", engine_render_addr );

    // so initially thought this shouldve been a single deref
    // but turns out its a double deref
    // going to the single deref in memory view in cheat engine
    // got me to an object, derefing that object again
    // in theory wouldve pointed me to a vtbl (wrong, no clue why),
    // tried to deref again, boom correct vtbl -> double deref for obj
    engine_renderer = **reinterpret_cast< crender *** >( engine_render_addr );

    // printf( "engine_renderer* addr=%p\n", engine_renderer );
    // printf( "addr off=%p ptr off=%p\n", mod->get_offset( engine_render_addr ), mod->get_offset( reinterpret_cast< uintptr_t >( engine_renderer ) ) );

    //const auto surface_addr = utils::get_module( "client.dll" )->get_offset_address( 0x2C830F + 0x02 );
    const auto csurf_addr = utils::scan_pattern( "client.dll", "8B 0D ? ? ? ? 52 8B 01 FF 50 ? E9" ) + 2;
    interfaces::c_surface = **reinterpret_cast< surface*** >( csurf_addr );

    //printf( "c_surface=%p\n", interfaces::c_surface );

    vgui_panel = utils::bruteforce_interface< uintptr_t >( "VGUI_Panel009" );

    assert( vgui_panel );
    printf( "VGUI_Panel009 obj @ -> %p\n", vgui_panel );

    printf( "[%s] done\n", __FUNCTION__ );
}
