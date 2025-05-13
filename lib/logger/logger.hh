#ifndef LOGGER_HH
#define LOGGER_HH

#include "Windows.h"

#include <format>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "color.hh"
#include "components/line.hh"
#include "components/list.hh"

/*
rewritten from:
https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

    ESC = 0x1B
ESC M -> reverse \n, maintains horizontal position, scrolls buffer if necessary
ESC 7 -> save cursor position in memory
ESC 8 -> restore cursor position from memory

cursor positioning stuff

param <n>
[opt] represents the distance to move
unset / 0 => treated as 1
<=32,767 (maximum short value) && >0

ESC[<n>A -> cursor up
ESC[<n>B -> cursor down
ESC[<n>C -> cursor right
ESC[<n>D -> cursor left

ESC[<n>E -> cursor down n lines
ESC[<n>F -> cursor up n lines
ESC[<n>G -> cursor to column n
ESC[<n>d -> cursor to row n

ESC[<y>;<x>f -> cursor to row y, column x

s & u are ansy.sys emulation for ESC 7 & 8

cursor visibility
ESC[?12h -> start blinking
ESC[?12l -> stop blinking
ESC[?25h -> show cursor
ESC[?25l -> hide cursor

cursor shape (SP = 0x20 (space))
ESC[0SPq -> default
ESC[1SPq -> blinking block
ESC[2SPq -> steady block
ESC[3SPq -> blinking underline
ESC[4SPq -> steady underline
ESC[5SPq -> blinking bar
ESC[6SPq -> steady bar

extended colors
ESC[38;2;<r>;<g>;<b>m -> set fg
ESC[48;2;<r>;<g>;<b>m -> set bg
ESC]4;<i>;rgb:<r>/<g>/<b>BEL (0x07) -> set color palette @ idx i

designate character set
ESC(0 -> enables DEC line drawing mode
ESC(1 -> enables ASCII mode (default)

j 	┘    k 	┐    l 	┌
m 	└    n 	┼    q 	─
t 	├    u 	┤    v 	┴
w 	┬    x 	│

setting titles
ESC]0;<title>BEL -> set window title
*/

/*
what I would want to achieve:
- simple wrappers for simple sequences
- rendering context/component structure for logging

methods
- log->list( vec<entry_t>& entries, int lines )
 */


class logger {
private:
    std::vector< std::unique_ptr< log::components::base > > m_components{ };

public:
    logger( ) {
        // clear console on init
        system( "cls" );
    };

    void setup( ) {
        // hide cursor
        std::cout << "\x1B[?25l";
    }

    // maybe redo to
    //
    // logger->line( ... )->prefix( ... )->spew( ); ?
    // logger->list( ... )->log( ... );
    // etc...
    //
    // imo looks cleaner

    log::components::line* line( const std::string& text ) {
        m_components.emplace_back( std::make_unique< log::components::line >( text ) );
        return static_cast< log::components::line* >(  m_components.back( ).get( ) );
    }

    log::components::list_t* list( const size_t lines ) {
        m_components.emplace_back( std::make_unique< log::components::list_t >( lines ) );
        return static_cast< log::components::list_t* >( m_components.back( ).get( ) );
    }

    /*template< typename Component, typename... Args >
    Component* add( Args&&... args ) {
        // we add an element
        m_components.emplace_back( std::make_unique< Component >( std::forward< Args >( args )... ) );

        const auto last = m_components.back( ).get( );

        // log last to update cursor pos
        last->spew( );

        // move cursor down
        std::cout << "\n";

        return static_cast< Component* >( last );
    }*/
};

inline logger g_log;


#endif //LOGGER_HH
