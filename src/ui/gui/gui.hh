#ifndef GUI_HH
#define GUI_HH

#include <array>
#include <vector>

#include "tab.hh"

class base_gui {
private:
    size_t m_selected_tab { 0 };
    std::vector< gui::tab > m_tabs { };

    int m_sidebar_width { 100 };

    std::array< int, 2 > m_pos { 200, 200 };
    std::array< int, 2 > m_tabs_start { 200, 200 };

    std::array< int, 2 > m_size { 400, 300 };
    std::array< int, 2 > m_min_size { 400, 300 };
    std::array< int, 2 > m_max_size { 800, 600 };

    void draw_element( base_component* );
public:
    base_gui( );

    void draw( );

    void set_cursor_pos( int x, int y );
    int get_max_width( );
    void set_context_width( int new_w );

    gui::tab* get_selected_tab( );

    gui::tab* tab( const std::string& name );
};

inline base_gui g_gui;


#endif //GUI_HH
