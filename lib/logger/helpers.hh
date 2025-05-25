#ifndef HELPERS_HH
#define HELPERS_HH

#include "Windows.h"
#include <array>

namespace helpers {
    inline CONSOLE_SCREEN_BUFFER_INFO console_info { };
    inline std::array< SHORT, 2 > cursor_pos{ 0, 0 };
}

inline HANDLE get_console( ) {
    return GetStdHandle( STD_OUTPUT_HANDLE );
}

inline CONSOLE_SCREEN_BUFFER_INFO& get_console_info( ) {
    GetConsoleScreenBufferInfo( get_console( ), &helpers::console_info );
    return helpers::console_info;
}

inline std::array< SHORT, 2 > get_cursor_pos( ) {
    const auto ci = get_console_info( );
    helpers::cursor_pos[ 0 ] = ci.dwCursorPosition.X;
    helpers::cursor_pos[ 1 ] = ci.dwCursorPosition.Y;

    return helpers::cursor_pos;
}

#endif //HELPERS_HH
