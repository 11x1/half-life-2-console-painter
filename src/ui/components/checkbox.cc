#include "checkbox.hh"

#include "../gui_context.hh"
#include "../../input/input.hh"
#include "../../renderer/surface_wrapper.hh"

using namespace component;

void checkbox::draw( ) {
    const auto pos = Vector( gui_context::m_cursor );
    const auto size = Vector( gui_context::m_ctx_width, 20 );

    handle( pos, size );

    surface_wrapper::draw_rectangle_filled( pos, Vector( 20, 20 ), Color( 0, 0, 0, 255 ) );
    surface_wrapper::draw_rectangle_filled( Vector( pos.x + 4, pos.y + 4 ), Vector( 12, 12 ), Color( 255, 255, 255, m_alpha_animator.get_value( ) ) );

    static auto ws_name = std::wstring( m_name.begin( ), m_name.end( ) );
    surface_wrapper::draw_text( ws_name, Vector( pos.x + 25, pos.y ), Color( 255, 255, 255, m_alpha_animator.get_value( ) ), fonts::element_name );

    // m_alpha_animator.draw_debug( Vector( 100, 80 ) );
}

void checkbox::handle( const Vector& pos, const Vector& size ) {
    const bool in_bounds = input_handler::is_mouse_in_bounds( pos, size );

    if ( in_bounds != m_hovered ) {
        m_hovered = in_bounds;

        if ( !*m_checked ) {
            if ( m_hovered ) {
                m_alpha_animator.update_end( 50 );
            } else {
                m_alpha_animator.update_end( 0 );
            }
        }

    }

    if ( m_hovered && input.is_key_released( VK_LBUTTON ) ) {
        *m_checked = !*m_checked;
        m_alpha_animator.update_end( *m_checked ? 255 : 50 );
    }
}

void checkbox::draw_debug( ) {

}
