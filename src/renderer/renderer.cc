#include "renderer.hh"

void renderer::setup( ) {
    fonts::watermark_font = renderer::create_font( "Arial", 14, 300, FONTFLAG_ANTIALIAS );
    fonts::entity_esp = renderer::create_font( "Arial", 10, 300, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE );


    fonts::element_name = renderer::create_font( "Arial", 12, 300, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );


    fonts::tab_font = renderer::create_font( "ProggyCleanTT", 16, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );
    fonts::subtab_font = renderer::create_font( "ProggyCleanTT", 16, 400, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );

    fonts::debug = renderer::create_font( "ProggyCleanTT", 12, 400, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE );
}