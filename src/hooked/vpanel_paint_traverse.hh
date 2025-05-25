#ifndef VPANEL_PAINT_TRAVERSE_HH
#define VPANEL_PAINT_TRAVERSE_HH

#include "../hooks/hooks.hh"
#include "../debug/debug.hh"

#include <cstdint>
#include <fstream>

#include "../features/console_paint.hh"

MAKE_VFTABLE_HOOK( vpanel_paint_traverse, 41,
                   void, __fastcall, void* a1, void* edx, void* a3, bool a4, bool a5 ) {
    /* old lua code, although for csgo
            local VPanel = ffi.cast( "uintptr_t**", a3 )
            local name = get_vtbl_func( VPanel, 35, "const char*(__thiscall*)(void*)" )( VPanel )
    */

    const auto name = utils::get_vfunc< const char*( __thiscall* )( void* ) >( a3, 35 )( a3 );
    debug::note_call( name );

    if ( strcmp( name, "CGameConsoleDialog" ) == 0 ) {
        console_paint::update_console_parent_pos( a3 );
    }

    if ( strcmp( name, "CConsolePanel" ) == 0 ) {
        console_paint::run( a1, edx, a3 );
    }

    original( a1, edx, a3, a4, a5 );
};

#endif //VPANEL_PAINT_TRAVERSE_HH
