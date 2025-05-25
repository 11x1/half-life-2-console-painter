#ifndef LOG_HH
#define LOG_HH

#include "logger/logger.hh"

namespace logger {
    namespace prefixes {
        inline components::prefix info( "info", color( 0x5d6fc9 ) );
        inline components::prefix error( "error", color( 0xe16666 ) );
        inline components::prefix success( "success", color( 0x66e166 ) );
        inline components::prefix debug( "debug", color( 0x40 ) );
        inline components::prefix warning( "warning", color( 0xffe699 ) );
    }

    inline components::list_t* last_list{ nullptr };
    inline components::line* last_list_status{ nullptr };

    inline void on_uninject( ) {
        last_list = nullptr;
        last_list_status = nullptr;
    }
}

#define LOG( level, fmt, ... ) {\
    g_log.line( \
        std::format( fmt __VA_OPT__(,) __VA_ARGS__ ) \
    )->prefix( logger::prefixes::level )->prefix( logger::components::prefix( std::string( __FUNCTION__  ), color( 0x30 ) ) )->color( color( 0xC0 ) )->spew( ); \
}

#define START_LIST( status_prefix, status_message, lines, list_prefix ) {\
    logger::last_list = g_log.list( lines )->prefix( list_prefix )->spew( ); \
    logger::last_list_status = g_log.line( status_message )->prefix( status_prefix )->spew( ); \
}

#define LOG_LIST( status_fmt, ... ) if ( logger::last_list != nullptr ) {\
    logger::last_list->line( std::format( status_fmt __VA_OPT__(,) __VA_ARGS__ ) );\
}

#define END_LIST( status_prefix, status_fmt, ... ) if ( logger::last_list_status != nullptr ) {\
    logger::last_list_status->update_entry( \
        *logger::components::entry_t( \
            std::format( status_fmt __VA_OPT__(,) __VA_ARGS__ ) \
        ).prefix( status_prefix ) \
    ); \
    logger::last_list_status = nullptr; \
    logger::last_list = nullptr; \
}

#endif //LOG_HH
