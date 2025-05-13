#include "Windows.h"

#include "gui.hh"

#include "../../sdk/globals.hh"
#include "../../input/input.hh"
#include "../../renderer/surface_wrapper.hh"
#include "../gui_context.hh"
#include "../../config/config.hh"

inline base_gui::base_gui( ) {
    this->tab( "tabber" )->subtab( "subtabber" )->checkbox( "siema", &config::test_feature );
}

void base_gui::draw( ) {
    // draw bg
    surface_wrapper::draw_rectangle_filled( Vector( m_pos ), Vector( m_size ), GUI_BACKGROUND );
    surface_wrapper::draw_rectangle( Vector( m_pos ), Vector( m_size ), GUI_OUTLINE );

    // draw tab texts text
    for ( size_t i = 0; i < m_tabs.size( ); i++ ) {
        const auto tab = &m_tabs[ i ];
        const auto tabpos = Vector( m_pos[ 0 ], m_pos[ 1 ] + i * 18 );
        const bool hovered = input.is_mouse_in_bounds( tabpos, Vector( m_sidebar_width, 18 ) );

        surface_wrapper::draw_text(
            tab->get_wstring_name( ),
            tabpos,
            m_selected_tab == i ? GUI_TAB_SELECTED_TEXT : hovered ? GUI_TAB_HOVER_TEXT : GUI_TAB_IDLE_TEXT,
            fonts::tab_font
        );

        if ( input.is_key_pressed( VK_LBUTTON ) && hovered ) {
            m_selected_tab = i;
        }
    }

    // draw tabs
    const auto selected_tab = &m_tabs[ m_selected_tab ];

    for ( size_t i = 0; i < selected_tab->get_subtab_count( ); i++ ) {
        const auto subtab = selected_tab->get_subtab( i );
        const auto subtab_pos = Vector( m_pos[ 0 ] + m_sidebar_width, m_pos[ 1 ] + i * 18 );
        const bool hovered = input.is_mouse_in_bounds( subtab_pos, Vector( m_sidebar_width, 18 ) );

        surface_wrapper::draw_text(
            subtab->get_wstring_name( ),
            subtab_pos,
            selected_tab->get_selected_subtab( ) == i
                ? GUI_TAB_SELECTED_TEXT
                : hovered
                      ? GUI_TAB_HOVER_TEXT
                      : GUI_TAB_IDLE_TEXT,
            fonts::subtab_font
        );

        if ( input.is_key_pressed( VK_LBUTTON ) && hovered ) {
            selected_tab->set_selected_subtab( i );
        }
    }

    // draw elements (initilly only for selected subtab)
    const auto selected_subtab = selected_tab->get_subtab( selected_tab->get_selected_subtab( ) );

    set_cursor_pos( m_pos[ 0 ] + m_sidebar_width * 2, m_pos[ 1 ] );
    for ( auto& el : selected_subtab->get_components( ) ) {
        draw_element( el.get( ) );
    }
}

void base_gui::draw_element( base_component* el ) {
    el->draw( );
    gui_context::m_cursor[ 1 ] += el->get_height( );
}

void base_gui::set_cursor_pos( int x, int y ) {
    gui_context::m_cursor[ 0 ] = x;
    gui_context::m_cursor[ 1 ] = y;
}

int base_gui::get_max_width( ) {
    return gui_context::m_ctx_width;
}

void base_gui::set_context_width( const int new_w ) {
    gui_context::m_ctx_width = new_w;
}

gui::tab* base_gui::tab( const std::string& name ) {
    m_tabs.push_back( gui::tab( name ) );
    return &m_tabs.at( m_tabs.size( ) - 1 );
}

gui::tab* base_gui::get_selected_tab( ) {
    return &m_tabs[ m_selected_tab ];
}
