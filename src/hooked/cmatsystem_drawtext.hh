#ifndef CMATSYSTEM_DRAWTEXT_HH
#define CMATSYSTEM_DRAWTEXT_HH

#include <format>
#include <string>

namespace cmatsystem_drawtext {
    using def = void ( __cdecl* )( void*, int, int, int, int r, int g, int b, int a, const char* fmt, ... );
    inline def original{ nullptr };
    inline size_t index = 0x288 / 4;

    inline void __cdecl hook( void* thisptr, int font, int x, int y, int r, int g, int b, int a, const char* fmt, ... ) {
        char buffer[ 4096 ];
        va_list args;
        va_start( args, fmt );
        vsnprintf( buffer, sizeof( buffer ), fmt, args );
        va_end( args );

        auto sigma_sigma_boy = ( surface* )thisptr;

        std::string text{ buffer };
        if ( *fmt == 'f' && *( fmt + 1 ) == 'p' && *( fmt + 2 ) == 's' ) {
            sigma_sigma_boy->set_render_font( font );
            sigma_sigma_boy->set_text_color( 225, 102, 102, 255 );
            sigma_sigma_boy->set_text_pos( Vector( x, y ) );
            sigma_sigma_boy->draw_text( L"[hl2 internal]" );

            x += sigma_sigma_boy->get_text_size( font, L"[hl2 internal] " ).x;
        }

        original( thisptr, font, x, y, r, g, b, a, text.c_str( ) );
    };
}

#endif //CMATSYSTEM_DRAWTEXT_HH
