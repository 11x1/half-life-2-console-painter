#include "input.hh"

void input_handler::check_key( const unsigned int key ) {
    if ( !m_key_states.contains( key ) )
        m_key_states[ key ] = key_state_t( );
}

bool input_handler::is_key_held( const unsigned int key ) {
    check_key( key );

    // from https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
    //  If the most significant bit is set, the key is down,
    //  and if the least significant bit is set, the key was
    //  pressed after the previous call to GetAsyncKeyState.
    //
    // returns short -> 2 bytes -> 0000 0000 0000 0000
    // msb = leftmost; 1000 0000 0000 0000 = 0x8000
    const auto new_held = ( GetAsyncKeyState( key ) & 0x8000 ) != 0;
    const auto state = &m_key_states[ key ];

    // held? set pressed to true
    // held? pressed? set pressed to false;

    state->is_pressed = true;

    // not held on first call
    // held on second call, mark unpress
    if ( state->is_held ) {
        state->is_pressed = false;
    }

    // not held on second call, mark release
    if ( !new_held && state->is_held ) {
        state->is_released = true;
    }

    state->is_held = new_held;

    return state->is_held;
}

bool input_handler::is_key_pressed( const unsigned int key ) {
    // update state via keyheld
    is_key_held( key );
    return m_key_states[ key ].is_pressed;
}

bool input_handler::is_key_released( const unsigned int key ) {
    check_key( key );
    return m_key_states[ key ].is_released;
}

bool input_handler::is_mouse_in_bounds( const Vector& pos2d, const Vector& sz2d ) {
    static POINT mouse_pos { };
    GetCursorPos( &mouse_pos );

    return pos2d.x <= mouse_pos.x && mouse_pos.x <= pos2d.x + sz2d.x &&
           pos2d.y <= mouse_pos.y && mouse_pos.y <= pos2d.y + sz2d.y;
};
