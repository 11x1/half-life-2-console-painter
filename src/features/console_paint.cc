#include "console_paint.hh"

#include "../config/config.hh"
#include "../internal/interfaces.hh"
#include "../sdk/vgui/richtext.hh"
#include "../input/input.hh"

void console_paint::update_console_parent_pos( void* a3 ) {
    const auto panel_pos = interfaces::vgui_panel->get_panel_pos( a3 );
    game_console_dialog_pos[ 0 ] = panel_pos.x;
    game_console_dialog_pos[ 1 ] = panel_pos.y;
}

void console_paint::setup_dialog(void* a3, cconsolepanel* console_panel ) {
    LOG( debug, "CConsolePanel=0x{:X}", reinterpret_cast< uintptr_t >( console_panel ) );
    has_init = true;

    auto* hist = console_panel->m_pHistory;

    // set up buffer
    // clear all formatting done by the game
    for ( int i = hist->m_text_buffer.Count( ) - 1; i >= 0; i-- ) {
        if ( hist->m_text_buffer[ i ] == L'\n' || hist->m_text_buffer[ i ] == L'\r' )
            console_panel->m_pHistory->m_text_buffer.Remove( i );
    }

    const auto panel_size = interfaces::vgui_panel->get_panel_size( a3 );
    const auto panel_pos = interfaces::vgui_panel->get_panel_pos( a3 );
    LOG( info, "console panel size: {},{}", panel_size.x, panel_size.y );
    LOG( info, "console panel pos: {},{}", panel_pos.x, panel_pos.y );

    // clear the newline buffer so we can add our own
    hist->m_line_breaks.RemoveAll( );

    for ( int i = 0; i < max_chars_y; i++ ) {
        // we start at 99 so no extra logic is needed to take color calc into account for newlines
        // first at idx 101 (after 100)
        // second at idx 202 (after 201)
        // third at idx 303 (after 302)

        // i = 0; 1*100 + 0 = 0 -> after 100
        // i = 1; 2*100 + 1 = 201 -> after 201
        // i = 2; 3*100 + 2 = 302 -> after 302
        // etc
        const int line_idx = ( i + 1 ) * max_chars_x + i;
        hist->m_text_buffer.InsertAfter( line_idx, L'\n' );
    }


    // let's make a circle with the diameter
    // of line_max_chars
    // before clear all formatting done by the game

    hist->m_format_vector.RemoveAll( );

    // and then set the colors
    // also basically sets up the whole buffer to draw on later
    // just need to index at where mouse // sz is
    for ( int i = 0; i < max_chars_total; i++ ) {
        const int x = i % max_chars_x;
        const int y = i / max_chars_y;

        const bool in_rect = x >= 20 && x <= max_chars_x - 20
                              && y >= 20 && y <= max_chars_y - 20;

        TFormatStream fs;
        fs.color = in_rect ? Color( 0xFF0000FF ) : Color( 0xFFFFFFFF );
        fs.fade = TFade{ 0, -1, 0, 0xFF };
        fs.pixelsIndent = 0;
        fs.textClickable = false;
        fs.textStreamIndex = i;
        fs.m_sClickableTextAction = -1;

        hist->m_format_vector.InsertAfter( i - 1, fs );
    }

    TFormatStream white;
    white.color = Color( 0xFFFFFFFF );
    white.fade = TFade{ 0, -1, 0, 0xFF };
    white.pixelsIndent = 0;
    white.textClickable = false;
    white.textStreamIndex = max_chars_total + 1;
    white.m_sClickableTextAction = -1;

    for ( int i = 0; i < 100; i++ )
        hist->m_format_vector.InsertAfter( max_chars_total + 1, white );
}

void console_paint::run( void* a1, void* edx, void* a3 ) {
    // returns a IClientPanel*
    // but since CConsolePanel inherits
    // from IClientPanel, we can cast it
    const auto console_panel = utils::get_vfunc< cconsolepanel* ( __fastcall
        * )( void*, void*, void* ) >( a1, 244 / 4 )( a1, edx, a3 );


    if ( !has_init ) {
        setup_dialog( a3, console_panel );
    }


    auto* hist = console_panel->m_pHistory;

    static bool init_format{ false };

    // initialize console as almost black
    if ( !init_format ) {
        init_format = true;

        for ( auto& f : format_vector ) {
            f[ 0 ] = 0x10;
            f[ 1 ] = 0x10;
            f[ 2 ] = 0x10;
            f[ 3 ] = 0xFF;
        }
    }

    // loop through all chars and set the color
    // based on out "painted" colors
    for ( int i = 0; i < max_chars_total; i++ ) {
        const auto& c = format_vector[ i ];
        hist->m_format_vector[ i ].color[ 0 ] = c[ 0 ];
        hist->m_format_vector[ i ].color[ 1 ] = c[ 1 ];
        hist->m_format_vector[ i ].color[ 2 ] = c[ 2 ];
        hist->m_format_vector[ i ].color[ 3 ] = c[ 3 ];
    }

    // get mouse hovering idx
    auto mouse_pos = input_handler::get_mouse_pos( );

    if ( mouse_pos[ 0 ] > game_console_dialog_pos[ 0 ] && mouse_pos[ 1 ] > game_console_dialog_pos[ 1 ] ) {
        // console has left and top padding, #fix
        mouse_pos[ 1 ] -= game_console_dialog_pos[ 1 ] + 30;
        mouse_pos[ 0 ] -= game_console_dialog_pos[ 0 ] + 13;

        // hovered letter index, tried using gettextsize from
        // surface with richtext font but didn't work (size was bogus)
        const auto m_rel = Vector( mouse_pos[ 0 ] / 6, mouse_pos[ 1 ] / 11 );

        // calculate the index based on chars on x (+ newline)
        // and y axis
        const auto idx = ( int ) ( m_rel.y * ( max_chars_x + 1 ) + m_rel.x );

        if ( m_rel.x <= max_chars_x && m_rel.y <= max_chars_y && idx < max_chars_total ) {
            const Color col = config::m_use_brush ? Color( 0xFF0000FF ) : Color( 0x101010FF );
            const int brush_size = config::m_brush_size / 2;

            if ( GetAsyncKeyState( VK_CAPITAL ) ) {
                for ( int y = -brush_size; y <= brush_size; y++ ) {
                    for ( int x = -brush_size; x <= brush_size; x++ ) {
                        const int idx2 = ( int ) ( ( m_rel.y + y ) * ( max_chars_x + 1 ) + ( m_rel.x + x ) );
                        if ( idx2 > 0 && idx2 < max_chars_total ) {
                            format_vector[ idx2 ][ 0 ] = col[ 0 ];
                            format_vector[ idx2 ][ 1 ] = col[ 1 ];
                            format_vector[ idx2 ][ 2 ] = col[ 2 ];
                            format_vector[ idx2 ][ 3 ] = col[ 3 ];
                        }
                    }
                }
            }
        }
    }
}
