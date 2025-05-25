#ifndef CENGIEVGUI_PAINT_HH
#define CENGIEVGUI_PAINT_HH
#include <chrono>

#include "../config/config.hh"
#include "../debug/debug.hh"
#include "../features/console_paint.hh"
#include "../features/entity_esp.hh"
#include "../features/watermark.hh"
#include "../hooks/hooks.hh"
#include "../internal/globals.hh"

#include "../ui/components/checkbox.hh"
#include "../ui/components/slider.hh"

#include "../input/input.hh"

MAKE_VFTABLE_HOOK( cenginevgui_paint, ( 0x3FC - 0x3C8 ) / 4,
    void, __fastcall,
    void* thisptr, void* edx, int mode
) {
    original( thisptr, edx, mode );

    // a lot of 2's and 5's
    // LOG( debug, "{}", mode );
    if ( mode == 2 ) {
        input.update_keys( );

        const auto time = std::chrono::steady_clock::now( );
        const long long time_ms = time.time_since_epoch( ).count( ) / 1000000;
        globals::m_current_time = time_ms;

        entity_esp::run( );
        hl2_watermark::run( );

        static auto use_brush_checkbox = component::checkbox( "brush", &config::m_use_brush, 0 );
        static auto brush_size = component::slider( "brush_size", &config::m_brush_size, 1, 5 );

        use_brush_checkbox.draw( console_paint::game_console_dialog_pos[ 0 ] + 100, console_paint::game_console_dialog_pos[ 1 ] - 30 );
        brush_size.draw( console_paint::game_console_dialog_pos[ 0 ] + 130, console_paint::game_console_dialog_pos[ 1 ] - 30 );

        debug::draw( );
    }
}

#endif //CENGIEVGUI_PAINT_HH
