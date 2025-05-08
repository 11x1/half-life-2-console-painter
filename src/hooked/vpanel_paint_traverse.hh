#ifndef VPANEL_PAINT_TRAVERSE_HH
#define VPANEL_PAINT_TRAVERSE_HH

#include "../features/entity_esp.hh"

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
            interfaces::c_surface->draw_filled_rect( Vector( 100.f, 100.f ), Vector( 100.f, 50.f ) );
            entity_esp::run( );
        }

        original( a1, a2, a3, a4, a5 );
    };
}

#endif //VPANEL_PAINT_TRAVERSE_HH
