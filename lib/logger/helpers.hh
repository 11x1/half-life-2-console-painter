#ifndef HELPERS_HH
#define HELPERS_HH

#include "Windows.h"
#include <array>

inline HANDLE get_console( ) {
    return GetStdHandle( STD_OUTPUT_HANDLE );
}

inline CONSOLE_SCREEN_BUFFER_INFO& get_console_info( ) {
    static CONSOLE_SCREEN_BUFFER_INFO console_info;
    GetConsoleScreenBufferInfo( get_console( ), &console_info );
    return console_info;
}

inline std::array< SHORT, 2 > get_cursor_pos( ) {
    const auto ci = get_console_info( );
    static std::array< SHORT, 2 > cursor_pos{ 0, 0 };
    cursor_pos[ 0 ] = ci.dwCursorPosition.X;
    cursor_pos[ 1 ] = ci.dwCursorPosition.Y;

    return cursor_pos;
}

#endif //HELPERS_HH
