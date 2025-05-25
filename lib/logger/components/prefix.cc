#include "prefix.hh"

#include <iostream>

std::string logger::components::prefix::get( ) const {
    static const auto bracket_color = color( 0x333333 );
    return std::format( "{}[{} {} {}]\x1B[0m ",
                        bracket_color.get_ansi_fg( ),
                        m_color.get_ansi_fg( ),
                        m_prefix.c_str( ),
                        bracket_color.get_ansi_fg( )
    );
}
