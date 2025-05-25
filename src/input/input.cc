#include "input.hh"

#include <cstdio>
#include <ranges>

static POINT mouse_pos_relative { };

void input_handler::update_key( const unsigned int key ) {
    // from https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
    //  If the most significant bit is set, the key is down,
    //  and if the least significant bit is set, the key was
    //  pressed after the previous call to GetAsyncKeyState.
    //
    // returns short -> 2 bytes -> 0000 0000 0000 0000
    // msb = leftmost; 1000 0000 0000 0000 = 0x8000
    const auto keystate = GetAsyncKeyState( key ); // separated due to how (i think) it functions
    const auto new_held = ( keystate & 0x8000 ) != 0;
    auto* state = &m_key_states[ key ];

    // held? set pressed to true
    // held? pressed? set pressed to false;

    if ( new_held && !state->is_held ) {
        // printf( "[%s] %d pressed (%x)\n", __FUNCTION__, key, keystate );
        state->is_pressed = true;
    } else { // !new_held || state->is_held => pressed = 0
        state->is_pressed = false;
    }

    // not held on second call, mark release
    if ( !new_held && state->is_held ) {
        // printf( "[%s] %d released (%x)\n", __FUNCTION__, key, keystate );
        state->is_released = true;
    } else { // new_held || !state_held => released = 0
        state->is_released = false;
    }

    state->is_held = new_held;
}

// called inside framesttagenotify
void input_handler::update_keys( ) {
    for ( const auto& key : m_key_states | std::views::keys )
        update_key( key );
}

void input_handler::check_key( const unsigned int key ) {
    if ( !m_key_states.contains( key ) )
        m_key_states[ key ] = key_state_t( );
}

bool input_handler::is_key_held( const unsigned int key ) {
    check_key( key );
    return m_key_states[ key ].is_held;
}

bool input_handler::is_key_pressed( const unsigned int key ) {
    check_key( key );
    return m_key_states[ key ].is_pressed;
}

bool input_handler::is_key_released( const unsigned int key ) {
    check_key( key );
    return m_key_states[ key ].is_released;
}

bool input_handler::is_mouse_in_bounds( const Vector& pos2d, const Vector& sz2d ) {
    GetCursorPos( &mouse_pos_relative );
    ScreenToClient( GetActiveWindow( ), &mouse_pos_relative );

    return pos2d.x <= mouse_pos_relative.x && mouse_pos_relative.x <= pos2d.x + sz2d.x &&
           pos2d.y <= mouse_pos_relative.y && mouse_pos_relative.y <= pos2d.y + sz2d.y;
}

std::array<int, 2> input_handler::get_mouse_pos( ) {
    GetCursorPos( &mouse_pos_relative );

    // basically (-win_px_off_x, -win_px_off_y)
    // subtracts from given, give it cursor pos
    // boom
    ScreenToClient( GetActiveWindow( ), &mouse_pos_relative );

    return { mouse_pos_relative.x, mouse_pos_relative.y };
};
