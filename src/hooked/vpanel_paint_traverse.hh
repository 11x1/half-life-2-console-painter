#ifndef VPANEL_PAINT_TRAVERSE_HH
#define VPANEL_PAINT_TRAVERSE_HH

#include "../hooks/hooks.hh"
#include "../debug/debug.hh"
#include "../internal/globals.hh"
#include "../features/entity_esp.hh"
#include "../features/watermark.hh"
#include "../ui/gui/gui.hh"

MAKE_VFTABLE_HOOK( vpanel_paint_traverse, 41,
                   void, __fastcall, int a1, int a2, void* a3, bool a4, bool a5 ) {
    /* old lua code, although for csgo
            local VPanel = ffi.cast( "uintptr_t**", a3 )
            local name = get_vtbl_func( VPanel, 35, "const char*(__thiscall*)(void*)" )( VPanel )
        */

    const auto name = utils::get_vfunc< const char*( __thiscall* )( void* ) >( a3, 35 )( a3 );

    const auto wstrname = std::wstring( name, name + strlen( name ) );
    if ( !debug::drawpanel_calls.contains( wstrname ) ) {
        debug::drawpanel_calls[ wstrname ] = 1;
    } else {
        debug::drawpanel_calls[ wstrname ] += 1;
    }

    if ( strcmp( name, "CHudViewport" ) == 0 ) {
        const auto time = std::chrono::steady_clock::now( );
        const long long time_ms = time.time_since_epoch( ).count( ) / 1000000;
        globals::m_current_time = time_ms;

        entity_esp::run( );
        hl2_watermark::run( );

        g_gui.draw( );

        debug::draw( );
    }

    original( a1, a2, a3, a4, a5 );
};

#endif //VPANEL_PAINT_TRAVERSE_HH
