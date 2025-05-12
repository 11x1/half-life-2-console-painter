#include "checkbox.hh"
#include "../../input/input.hh"
#include "../../renderer/surface_wrapper.hh"

void checkbox::draw( ) {
    handle( );

    surface_wrapper::draw_rectangle_filled( Vector( 100, 100 ), Vector( 20, 20 ), Color( 0, 0, 0, 255 ) );
    surface_wrapper::draw_rectangle_filled( Vector( 100 + 4, 100 + 4 ), Vector( 12, 12 ), Color( 255, 255, 255, m_alpha_animator.get_value( ) ) );

    static auto ws_name = std::wstring( m_name.begin( ), m_name.end( ) );
    surface_wrapper::draw_text( ws_name, Vector( 125, 100 ), Color( 255, 255, 255, m_alpha_animator.get_value( ) ), fonts::element_name );

    m_alpha_animator.draw_debug( Vector( 100, 80 ) );
}

void checkbox::handle( ) {
    const bool in_bounds = input_handler::is_mouse_in_bounds( Vector( 100.f, 100.f ), Vector( 20.f, 20.f ) );

    if ( in_bounds != m_hovered ) {
        m_hovered = in_bounds;

        if ( m_hovered ) {
            m_alpha_animator.update_end( 200 );
        } else {
            m_alpha_animator.update_end( 50 );
        }
    }

    if ( in_bounds && input.is_key_pressed( VK_LBUTTON ) ) {
        m_checked = !m_checked;
    }
}

void checkbox::draw_debug( ) {

}
