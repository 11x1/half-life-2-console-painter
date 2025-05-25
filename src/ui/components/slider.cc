#include "slider.hh"

#include "../../input/input.hh"

void component::slider::handle( const Vec2& pos, const Vec2& sz ) {
    if ( !input.is_mouse_in_bounds( pos, sz ) || !input.is_key_pressed( VK_LBUTTON ) ) return;

    const auto mouse_pos = input_handler::get_mouse_pos( );

    const auto off_x = mouse_pos[ 0 ] - pos.x;

    const auto delta = m_max - m_min;

    LOG( info, "delta: {}", delta );

    const int value_at_pc = ( off_x * delta ) / m_width;

    *m_ref_value = m_min + value_at_pc;

    m_value_animator.update_end( *m_ref_value );
}

void component::slider::draw_debug( ) {
}

void component::slider::draw( const int x, const int y ) {
    const auto pos = Vec2( x, y );
    const auto size = Vec2( m_width, 20 );

    handle( pos, size );

    // bg
    renderer::draw_rectangle_filled( pos, size, Color( 0, 0, 0, 255 ) );
    renderer::draw_rectangle( pos, size, Color( 255, 255, 255, 100 ) );

    const auto delta = m_max - m_min;
    const int lerped_value = ( m_value_animator.get_value( ) * ( m_width - 8 ) ) / delta;

    // draw inner
    renderer::draw_rectangle_filled( Vector( pos.x + 4, pos.y + 4 ), Vector( lerped_value, size.y - 8 ), Color( 255, 255, 255, 200 ) );
}
