#include "list.hh"

#include <iostream>

void log::components::list_t::line( entry_t entry ) {
    size_t longest_entry = 0;

    for ( const auto& e : m_entries ) {
        if ( e.length( ) > longest_entry ) {
            longest_entry = e.length( );
        }
    }

    // entry is moved into the list
    entry.prefix( m_prefix );
    m_entries.push_back( std::move( entry ) );

    // only need max_rows entries
    if ( m_entries.size( ) > m_max_rows ) {
        m_entries.erase( m_entries.begin( ) );
    }

    const auto before_update_cursor = get_cursor_pos( );
    SetConsoleCursorPosition( get_console( ), { m_cursor[ 0 ], static_cast< SHORT >( m_cursor[ 1 ] - m_scroll_offset ) } );
    for ( const auto& e : m_entries ) {
        std::cout << "\x1B[" << longest_entry << "X" << e.get_content( );
        std::cout << '\n';
    }
    SetConsoleCursorPosition( get_console( ), { before_update_cursor[ 0 ], before_update_cursor[ 1 ] } );

    // new entry, rerender
    // update( );
}

void log::components::list_t::line( const std::string& str ) {
    line(
        log::components::entry_t( str, m_default_color, COLOR_NONE, 0 )
    );
}

void log::components::list_t::color( const ::color col ) {
    m_default_color = col;
}

log::components::list_t* log::components::list_t::spew( ) {
    // spew out all entries
    // if no entry, add a palceholder (aka empty line)
    for ( size_t i = 0; i < m_max_rows; i++ ) {
        if ( i < m_entries.size( ) ) {
            m_entries[ i ].spew( );
        }

        // if ( i != m_max_rows - 1 )
            std::cout << std::endl;
    }

    return this;
}

void log::components::list_t::update( ) {
    // cache cursor position
    const auto before_update_cursor = get_cursor_pos( );

    // move to where our list is
    SetConsoleCursorPosition( get_console( ), { m_cursor[ 0 ], static_cast< SHORT >( m_cursor[ 1 ] - m_scroll_offset ) } );

    spew( );

    // restore cached pos
    SetConsoleCursorPosition( get_console( ), { before_update_cursor[ 0 ], before_update_cursor[ 1 ] } );
}

log::components::list_t* log::components::list_t::prefix( const std::string& prefix ) {
    m_prefix += prefix;
    for ( auto& e : m_entries ) {
        e.prefix( prefix );
    }
    return this;
}

log::components::list_t* log::components::list_t::prefix( const log::components::prefix& prefix ) {
    return this->prefix( prefix.get( ) );
}
