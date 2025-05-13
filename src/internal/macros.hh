#ifndef MACROS_HH
#define MACROS_HH

#include <iostream>

#include "Windows.h"
#include <string>

#include "globals.hh"

inline void log_brackets( const std::string& what, const int col ) {
    SetConsoleTextAttribute( globals::console_handle, FG_GRAY );
    std::cout << "[";
    SetConsoleTextAttribute( globals::console_handle, col );
    std::cout << what.c_str( );
    SetConsoleTextAttribute( globals::console_handle, FG_GRAY );
    std::cout << "]";
    SetConsoleTextAttribute( globals::console_handle, FG_LIGHTGRAY );
}

inline size_t last_log_length{ 0 };
inline bool last_logged_load{ false };

/*
    if ( last_logged_load ) { \
        std::cout << "\r"; \
        for ( size_t i = 0; i < last_log_length; i++ ) { \
            std::cout << " "; \
        } \
        std::cout << "\r"; \
    } \
    log_brackets( " loading ", FG_LIGHTGRAY | BG_GRAY ); \
    std::cout << " "; \
    const auto fnname = std::format( " {} ", __FUNCTION__ ); \
    log_brackets( fnname, FG_BLACK | BG_GRAY ); \
    std::cout << " "; \
    const auto fmted = std::format( fmt __VA_OPT__(,) __VA_ARGS__ ); \
    std::cout << fmted.c_str( ); \
    last_log_length = 9 + 1 + fnname.size( ) + 1 + fmted.length( ); \
    last_logged_load = true; \ */

#define LOG_LOADING( fmt, ... ) {\
    if ( last_logged_load ) { \
        /* clear previous text */ \
        CONSOLE_SCREEN_BUFFER_INFO csbi;\
        GetConsoleScreenBufferInfo( globals::console_handle, &csbi );\
        DWORD written;\
        DWORD fill_count = csbi.dwSize.X;\
        COORD home_coord = { 0, csbi.dwCursorPosition.Y };\
        FillConsoleOutputCharacter( globals::console_handle, ' ', fill_count, home_coord, &written );\
        SetConsoleCursorPosition( globals::console_handle, home_coord );\
    }\
    log_brackets ( " loading ", FG_LIGHTGRAY | BG_GRAY );\
    std::cout << " ";\
    const auto fnname = std::format( " {} ", __FUNCTION__ );\
    log_brackets( fnname, FG_BLACK | BG_GRAY );\
    std::cout<<" ";\
    SetConsoleTextAttribute( globals::console_handle, FG_LIGHTGRAY );\
    const auto fmted = std::format( fmt __VA_OPT__(,  ) __VA_ARGS__ ); \
    std::cout<<fmted.c_str( );\
    last_log_length = 9 + 1 + fnname.size( ) + 1 + fmted.length( ); \
    last_logged_load = true;\
}

#define LOG_LOADING_END( fmt, ... ) {\
    if ( last_logged_load ) { \
        /* clear previous text */ \
        CONSOLE_SCREEN_BUFFER_INFO csbi;\
        GetConsoleScreenBufferInfo( globals::console_handle, &csbi );\
        DWORD written;\
        DWORD fill_count = csbi.dwSize.X;\
        COORD home_coord = { 0, csbi.dwCursorPosition.Y };\
        FillConsoleOutputCharacter( globals::console_handle, ' ', fill_count, home_coord, &written );\
        SetConsoleCursorPosition( globals::console_handle, home_coord );\
    }\
    log_brackets ( " success ", FG_LIGHTGREEN | BG_GREEN );\
    std::cout << " ";\
    const auto fnname = std::format( " {} ", __FUNCTION__ );\
    log_brackets( fnname, FG_BLACK | BG_GRAY );\
    std::cout<<" ";\
    SetConsoleTextAttribute( globals::console_handle, FG_LIGHTGRAY );\
    const auto fmted = std::format( fmt __VA_OPT__(,  ) __VA_ARGS__ ); \
    std::cout<<fmted.c_str( ) << std::endl; \
    last_log_length = 9 + 1 + fnname.size( ) + 1 + fmted.length( ) + 1; \
    last_logged_load = false; \
}

#define LOG_INFO( fmt, ... ) {\
    log_brackets( " info ", FG_CYAN | BG_NAVYBLUE ); \
    std::cout << " "; \
    const auto fnname = std::format( " {} ", __FUNCTION__ ); \
    log_brackets( fnname, FG_BLACK | BG_GRAY ); \
    std::cout << " "; \
    SetConsoleTextAttribute( globals::console_handle, FG_LIGHTGRAY );\
    const auto fmted = std::format( fmt __VA_OPT__(,) __VA_ARGS__ ); \
    std::cout << fmted.c_str( ) << std::endl; \
    last_log_length = 6 + 1 + fnname.size( ) + 1 + fmted.length( ) + 1; \
    last_logged_load = false; \
}

#define LOG_DEBUG( fmt, ... ) {\
    log_brackets( " debug ", FG_LIGHTGRAY | BG_GRAY ); \
    std::cout << " "; \
    const auto fnname = std::format( " {} ", __FUNCTION__ ); \
    log_brackets( fnname, FG_BLACK | BG_GRAY ); \
    std::cout << " "; \
    SetConsoleTextAttribute( globals::console_handle, FG_GRAY );\
    const auto fmted = std::format( fmt __VA_OPT__(,) __VA_ARGS__ ); \
    std::cout << fmted.c_str( ) << std::endl; \
    last_log_length = 7 + 1 + fnname.size( ) + 1 + fmted.length( ) + 1; \
    last_logged_load = false; \
    SetConsoleTextAttribute( globals::console_handle, FG_WHITE );\
}

#define LOG_ERROR( fmt, ... ) {\
    log_brackets( " error ", FG_RED | BG_MAROON ); \
    std::cout << " "; \
    const auto fnname = std::format( " {} ", __FUNCTION__ ); \
    log_brackets( fnname, FG_BLACK | BG_GRAY ); \
    std::cout << " "; \
    const auto fmted = std::format( fmt __VA_OPT__(,) __VA_ARGS__ ); \
    std::cout << fmted.c_str( ) << std::endl; \
    last_log_length = 7 + 1 + fnname.size( ) + 1 + fmted.length( ) + 1; \
    last_logged_load = false; \
}

#define LOG_SUCCESS( fmt, ... ) {\
    log_brackets( " success ", FG_LIGHTGREEN | BG_GREEN ); \
    std::cout << " "; \
    const auto fnname = std::format( " {} ", __FUNCTION__ ); \
    log_brackets( fnname, FG_BLACK | BG_GRAY ); \
    std::cout << " "; \
    const auto fmted = std::format( fmt __VA_OPT__(,) __VA_ARGS__ ); \
    std::cout << fmted.c_str( ) << std::endl; \
    last_log_length = 9 + 1 + fnname.size( ) + 1 + fmted.length( ) + 1; \
    last_logged_load = false; \
}

#endif //MACROS_HH
