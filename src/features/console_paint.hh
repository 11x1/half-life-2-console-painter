#ifndef CONSOLE_PAINT_HH
#define CONSOLE_PAINT_HH

#include <array>
#include <vector>

#include "../sdk/common/color.hh"
#include "../sdk/vgui/cconsolepanel.hh"

namespace console_paint {
    inline bool has_init { false };
    inline std::array< int, 2 > game_console_dialog_pos{ 0, 0 };

    // for video rendering:
    // 160 was close, but double
    // 159 is perfect but double (159 chars + newline)
    // 160 / 2 = 80 -> -1 newline, boom 79
    inline constexpr int max_chars_x = 79;
    inline constexpr int max_chars_y = 80;

    // total is ( x + 1 ) * y, need to include newline
    inline constexpr int max_chars_total = ( max_chars_x + 1 ) * max_chars_y;

    inline std::vector< Color > format_vector( max_chars_total );

    void update_console_parent_pos( void* gameconsoledialog );
    void setup_dialog( void* a3, cconsolepanel* );

    void run( void* a1, void* edx, void* a3 );
};



#endif //CONSOLE_PAINT_HH
