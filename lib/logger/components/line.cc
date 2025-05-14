#include "line.hh"

#include <iostream>

void log::components::line::update_entry( log::components::entry_t entry )  {
    // ESC [ <n> X -> erase n chars @ cursor
    const auto before_update_cursor = get_cursor_pos( );
    SetConsoleCursorPosition( get_console( ), { m_cursor[ 0 ], static_cast< SHORT >( m_cursor[ 1 ] - m_scroll_offset ) } );
    printf( "\x1B[%dX", m_entry.length( ) );
    SetConsoleCursorPosition( get_console( ), { before_update_cursor[ 0 ], before_update_cursor[ 1 ] } );

    m_entry = std::move( entry );
    update( );
}

void log::components::line::update_entry( const std::string& text ) {
    // ESC [ <n> X -> erase n chars @ cursor
    const auto before_update_cursor = get_cursor_pos( );
    SetConsoleCursorPosition( get_console( ), { m_cursor[ 0 ], static_cast< SHORT >( m_cursor[ 1 ] - m_scroll_offset ) } );
    printf( "\x1B[%dX", m_entry.length( ) );
    SetConsoleCursorPosition( get_console( ), { before_update_cursor[ 0 ], before_update_cursor[ 1 ] } );

    m_entry.set_text( text );
    update( );
}

log::components::line* log::components::line::color( const ::color& fg, const ::color& bg ) {
    m_entry.set_fg( fg );
    m_entry.set_bg( bg );
    return this;
}

log::components::line* log::components::line::spew( ) {
    m_entry.spew( );
    printf( "\n" );

    return this;
}

void log::components::line::update( ) {
    // cache cursor position
    const auto before_update_cursor = get_cursor_pos( );

    // move to where our line is
    SetConsoleCursorPosition( get_console( ), { m_cursor[ 0 ], static_cast< SHORT >( m_cursor[ 1 ] - m_scroll_offset ) } );

    spew( );

    // restore cached pos
    SetConsoleCursorPosition( get_console( ), { before_update_cursor[ 0 ], before_update_cursor[ 1 ] } );
}

log::components::line* log::components::line::prefix( const std::string& prefix ) {
    m_entry.prefix( prefix );
    return this;
}

log::components::line* log::components::line::prefix( const components::prefix& prefix ) {
    return this->prefix( prefix.get( ) );
}