#include "checkbox.hh"

#include "../../input/input.hh"
#include "../../renderer/renderer.hh"
#include "../../renderer/vec2.hh"

using namespace component;

void checkbox::draw( const int x, const int y ) {
    const auto pos = Vec2( x, y );
    const auto size = Vec2( 20, 20 );

    handle( pos, size );

    renderer::draw_rectangle_filled( pos, Vector( 20, 20 ), Color( 0, 0, 0, 255 ) );
    renderer::draw_rectangle_filled( Vector( pos.x + 4, pos.y + 4 ), Vector( 12, 12 ), Color( 255, 255, 255, m_alpha_animator.get_value( ) ) );

    static auto ws_name = std::wstring( m_name.begin( ), m_name.end( ) );
    renderer::draw_text( ws_name, Vector( pos.x + 25, pos.y ), Color( 255, 255, 255, m_alpha_animator.get_value( ) ), fonts::element_name );

    if ( m_texture_id ) {
        renderer::draw_texture( m_texture_id, Vector( pos.x + 5, pos.y + 5 ), Vector( 10, 10 ) );
    }
}

void checkbox::handle( const Vec2& pos, const Vec2& size ) {
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

    if ( m_hovered && input.is_key_pressed( VK_LBUTTON ) ) {
        *m_checked = !*m_checked;
        m_alpha_animator.update_end( *m_checked ? 255 : 50 );
    }
}

void checkbox::draw_debug( ) {

}
