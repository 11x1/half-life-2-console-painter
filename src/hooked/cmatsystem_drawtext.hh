#ifndef CMATSYSTEM_DRAWTEXT_HH
#define CMATSYSTEM_DRAWTEXT_HH

#include <cmath>
#include <string>

MAKE_VFTABLE_HOOK( cmatsystem_drawtext, 0x288 / 4,
                   void, __cdecl, surface* thisptr, int font, int x, int y, int r, int g, int b, int a, const char* fmt,
                   ...
) {
    char buffer[ 4096 ];
    va_list args;
    va_start( args, fmt );
    vsnprintf( buffer, sizeof( buffer ), fmt, args );
    va_end( args );

    // no I have not heard about strcmp
    if ( *fmt == 'f' && *( fmt + 1 ) == 'p' && *( fmt + 2 ) == 's' ) {
        thisptr->set_render_font( font );
        thisptr->set_text_color( 139, 126, 214, std::abs( std::sin( globals::m_current_time / 1000.f ) ) * 254 );
        thisptr->set_text_pos( Vector( x, y ) );
        thisptr->draw_text( L"paint!!!!! " );

        x += thisptr->get_text_size( font, L"paint!!!!! " ).x;
    }

    original( thisptr, font, x, y, r, g, b, a, buffer );
}

#endif //CMATSYSTEM_DRAWTEXT_HH
