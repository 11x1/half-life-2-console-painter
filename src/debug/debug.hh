#ifndef DEBUG_HH
#define DEBUG_HH
#include <algorithm>
#include <map>
#include <string>

#include "../renderer/renderer.hh"

namespace debug {
    inline std::map< std::wstring, size_t > drawpanel_calls { };

    inline void note_call( const char* name ) {
        // update called list for panel paints
        const auto wstrname = std::wstring( name, name + strlen( name ) );
        if ( !drawpanel_calls.contains( wstrname ) ) {
            drawpanel_calls[ wstrname ] = 1;
        } else {
            drawpanel_calls[ wstrname ] += 1;
        }
    }

    inline void draw( ) {
        static auto fontsz = renderer::get_text_size( L"123", fonts::debug );

        bool reset_all { false };
        for ( const size_t& amt : drawpanel_calls | std::views::values ) {
            if ( amt > INT_MAX ) reset_all = true;
        }

        if ( reset_all ) {
            for ( size_t& amt : drawpanel_calls | std::views::values ) {
                amt = 0;
            }
        }

        std::vector< std::pair< std::wstring, size_t > > sorted_calls( drawpanel_calls.begin( ), drawpanel_calls.end( ) );
        std::ranges::sort( sorted_calls, []( const auto& a, const auto& b ) {
            return a.second > b.second;
        } );

        for ( size_t i = 0; i < sorted_calls.size( ); i++ ) {
            const auto& [ name, amt ] = sorted_calls[ i ];
            renderer::draw_text( std::format( L"{}: {}", name, amt ), Vector( 0, i * fontsz.y ), Color( 255, 255, 255, 100 ), fonts::debug );
        }
    };
}

#endif //DEBUG_HH
