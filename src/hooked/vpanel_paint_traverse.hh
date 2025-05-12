#ifndef VPANEL_PAINT_TRAVERSE_HH
#define VPANEL_PAINT_TRAVERSE_HH

#include "../internal/globals.hh"
#include "../features/entity_esp.hh"
#include "../features/watermark.hh"

namespace vpanel_paint_traverse {
    using def = void( __fastcall* )( int a1, int a2, void* a3, bool a4, bool a5 );
    inline def original { nullptr };
    inline size_t index = 41;
    inline void __fastcall hook( int a1, int a2, void* a3, bool a4, bool a5 ) {
        /* old lua code, although for csgo
            local VPanel = ffi.cast( "uintptr_t**", a3 )
            local name = get_vtbl_func( VPanel, 35, "const char*(__thiscall*)(void*)" )( VPanel )
        */

        const auto name = utils::get_vfunc< const char*( __thiscall* )( void* ) >( a3, 35 )( a3 );

        if ( strcmp( name, "CHudViewport" ) == 0 ) {
            const auto time = std::chrono::steady_clock::now( );
            const long long time_ms = time.time_since_epoch( ).count( ) / 1000000;
            globals::m_current_time = time_ms;

            interfaces::c_surface->draw_filled_rect( Vector( 100.f, 100.f ), Vector( 100.f, 50.f ) );
            entity_esp::run( );
            hl2_watermark::run( );
        }

        original( a1, a2, a3, a4, a5 );
    };
}

#endif //VPANEL_PAINT_TRAVERSE_HH
