#ifndef LOG_HH
#define LOG_HH

#include "logger/logger.hh"

namespace log {
    namespace prefixes {
        inline components::prefix info ( "info", color( 0x5d6fc9 ) );
        inline components::prefix error ( "error", color( 0x1e6666 ) );
        inline components::prefix success ( "success", color( 0x66e166 ) );
        inline components::prefix debug ( "debug", color( 0x40 ) );
        inline components::prefix warning ( "warning", color( 0xffe699 ) );
    }
}

#define LOG( level, fmt, ... ) {\
    g_log.line( \
        std::format( fmt __VA_OPT__(,) __VA_ARGS__ ) \
    )->prefix( log::prefixes::level )->prefix( log::components::prefix( std::string( __FUNCTION__  ), color( 0x30 ) ) )->color( color( 0xC0 ) )->spew( ); \
}

#endif //LOG_HH
