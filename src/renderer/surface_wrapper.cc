#include "surface_wrapper.hh"

void surface_wrapper::setup( ) {
    fonts::watermark_font = surface_wrapper::create_font( "Arial", 14, 300, FONTFLAG_ANTIALIAS );
    fonts::entity_esp = surface_wrapper::create_font( "Arial", 10, 300, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE );


    fonts::element_name = surface_wrapper::create_font( "Arial", 12, 300, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );


    fonts::tab_font = surface_wrapper::create_font( "ProggyCleanTT", 16, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );
    fonts::subtab_font = surface_wrapper::create_font( "ProggyCleanTT", 16, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );

    fonts::debug = surface_wrapper::create_font( "ProggyCleanTT", 8, 400, FONTFLAG_DROPSHADOW );
}

HFont surface_wrapper::create_font( const std::string& fontname, const int size, const int weight,
                                    const int flags ) {
    const auto font = interfaces::c_surface->create_font( );
    interfaces::c_surface->set_font_glyph_set( font, fontname, size, weight, 0, 0, flags );
    return font;
}

void surface_wrapper::draw_line( const Vector& start, const Vector& end, const Color& color ) {
    interfaces::c_surface->set_draw_color( color );
    interfaces::c_surface->draw_line( start, end );
}

void surface_wrapper::draw_poly_line( const std::vector<Vector>& points, const Color& color ) {
    interfaces::c_surface->set_draw_color( color );
    interfaces::c_surface->draw_poly_line( points );
}

int surface_wrapper::create_texture( const std::vector<byte>& rgba_bytes, const Vector& sz ) {
    const auto tx = interfaces::c_surface->create_texture( );
    interfaces::c_surface->set_texture_rgba( tx, rgba_bytes, sz.x, sz.y );
    return tx;
}

void surface_wrapper::draw_texture( const int tx_id, const Vector& pos, const Vector& size ) {
    interfaces::c_surface->set_draw_texture( tx_id );
    interfaces::c_surface->draw_textured_rect( pos, size );
}

bool surface_wrapper::is_texture_valid( const int tx_id ) {
    return interfaces::c_surface->is_texture_valid( tx_id );
}

bool surface_wrapper::delete_texture( const int tx_id ) {
    return interfaces::c_surface->delete_texture( tx_id );
}
