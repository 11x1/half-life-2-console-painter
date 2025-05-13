#ifndef SURFACE_WRAPPER_HH
#define SURFACE_WRAPPER_HH

#include "../internal/interfaces.hh"
#include "../sdk/surface.hh"

namespace fonts {
    inline HFont watermark_font { 0 };
    inline HFont entity_esp { 0 };

    inline HFont element_name { 0 };

    inline HFont tab_font { 0 };
    inline HFont subtab_font { 0 };

    inline HFont debug { 0 };
}

namespace surface_wrapper {
    void setup( );

    inline HFont create_font( const std::string& fontname, int size, int weight, int flags = FONTFLAG_NONE );

    inline void draw_rectangle_filled( const Vector& pos, const Vector& size, const Color& color ) {
        interfaces::c_surface->set_draw_color( color );
        interfaces::c_surface->draw_filled_rect( pos, size );
    };

    inline void draw_rectangle( const Vector& pos, const Vector& size, const Color& color ) {
        interfaces::c_surface->set_draw_color( color );
        interfaces::c_surface->draw_outlined_rect( pos, size );
    };

    inline void draw_line( const Vector& start, const Vector& end, const Color& color );
    inline void draw_poly_line( const std::vector< Vector >& points, const Color& color );

    inline void draw_text( const std::wstring& text, const Vector& pos, const Color& color, const HFont font ) {
        interfaces::c_surface->set_text_color( color );
        interfaces::c_surface->set_render_font( font );
        interfaces::c_surface->set_text_pos( pos );
        interfaces::c_surface->draw_text( text );
    };

    inline Vector get_text_size( const std::wstring& text, const HFont font ) {
        return interfaces::c_surface->get_text_size( font, text );
    };

    // inline void draw_text( const std::string& text, const Vector& pos, const Color& color, const HFont font ) {
    //     draw_text( std::wstring( text.begin( ), text.end( ) ), pos, color, font );
    // }

    inline int create_texture( const std::vector< byte >& rgba_bytes, const Vector& sz );
    inline void draw_texture( const int tx_id , const Vector& pos, const Vector& size );
    inline bool is_texture_valid( const int tx_id );
    inline bool delete_texture( const int tx_id );

    inline Vector get_screen_size( ) {
        return interfaces::c_surface->get_screen_size( );
    };
};



#endif //SURFACE_WRAPPER_HH
