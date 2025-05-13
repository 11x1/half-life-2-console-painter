#ifndef GUI_CONTEXT_HH
#define GUI_CONTEXT_HH

#include <array>

#include "../sdk/globals.hh"

inline const auto GUI_BACKGROUND = Color( 0x1d1d1dFF );
inline const auto GUI_OUTLINE = Color( 0x232323FF );
inline const auto GUI_TAB_IDLE_TEXT = Color( 0xadadadFF );
inline const auto GUI_TAB_HOVER_TEXT = Color( 0xc1c1c1FF );
inline const auto GUI_TAB_SELECTED_TEXT = Color( 0xddddddFF );

// could be a class with public members aswell
namespace gui_context {
    inline int m_ctx_width { 200 };
    inline std::array< int, 2 > m_cursor { 0, 0 };
}

#endif //GUI_CONTEXT_HH
