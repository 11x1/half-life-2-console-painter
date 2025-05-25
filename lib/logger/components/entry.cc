#include "entry.hh"

#include <iostream>

std::string logger::components::entry_t::get_content( ) const {
    std::string content = "\x1B[0m" + m_prefix;

    std::string flags;
    if ( m_flags & BOLD ) {
        flags += "\x1B[1m";
    }
    if ( m_flags & UNDERLINE ) {
        flags += "\x1B[4m";
    }

    if ( m_fg.is_set( ) ) {
        content += m_fg.get_ansi_fg( );
    }

    if ( m_bg.is_set( ) ) {
        content += m_bg.get_ansi_bg( );
    }

    content += flags + m_text + "\x1B[0m";

    return content;
}

size_t logger::components::entry_t::length( ) const {
    return m_text.length( ) + m_prefix.length( );
}

void logger::components::entry_t::spew( ) const {
    printf( get_content( ).c_str( ) );
}

logger::components::entry_t* logger::components::entry_t::prefix( const std::string& prefix ) {
    m_prefix = m_prefix + prefix;
    return this;
}

logger::components::entry_t* logger::components::entry_t::prefix( const logger::components::prefix& prefix ) {
    m_prefix += prefix.get( );
    return this;
}

void logger::components::entry_t::set_text( const std::string& text ) {
    m_text = text;
}

void logger::components::entry_t::set_fg( const color& fg ) {
    m_fg = fg;
}

void logger::components::entry_t::set_bg( const color& bg ) {
    m_bg = bg;
}
