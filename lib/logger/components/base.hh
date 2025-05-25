#ifndef BASE_HH
#define BASE_HH

#include "Windows.h"
#include <array>
#include <iostream>
#include <string>

#include "prefix.hh"
#include "../helpers.hh"

namespace logger::components {
    class base {
    protected:
        std::array< SHORT, 2 > m_cursor{ 0, 0 };
        SHORT m_scroll_offset{ 0 };
    public:
        base( ) {
            const auto cpos = get_cursor_pos( );

            m_cursor[ 0 ] = cpos[ 0 ];
            m_cursor[ 1 ] = cpos[ 1 ];
        };

        virtual ~base( ) = default;

        virtual void set_scroll_offset( const SHORT amt ) {
            m_scroll_offset = amt;
        };

        virtual void update( ) = 0;
        virtual base* spew( ) = 0;

        // can either append a string or a prefix ptr
        virtual base* prefix( ) = 0;
        virtual base* prefix( const std::string& prefix ) = 0;
        virtual base* prefix( const logger::components::prefix& prefix ) = 0;
    };
}

#endif //BASE_HH
