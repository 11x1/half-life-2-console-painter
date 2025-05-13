#ifndef DEBUG_HH
#define DEBUG_HH
#include <map>
#include <string>

#include "../renderer/surface_wrapper.hh"

namespace debug {
    inline std::map< std::wstring, size_t > drawpanel_calls { };

    inline void draw( ) {
        static auto fontsz = surface_wrapper::get_text_size( L"123", fonts::debug );

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
        std::sort( sorted_calls.begin( ), sorted_calls.end( ), []( const auto& a, const auto& b ) {
            return a.second > b.second;
        } );

        for ( size_t i = 0; i < sorted_calls.size( ); i++ ) {
            const auto& [ name, amt ] = sorted_calls[ i ];
            surface_wrapper::draw_text( std::format( L"{}: {}", name, amt ), Vector( 0, i * fontsz.y ), Color( 255, 255, 255, 100 ), fonts::debug );
        }
    };
}

#endif //DEBUG_HH
