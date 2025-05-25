#ifndef INPUT_HH
#define INPUT_HH

#include "Windows.h"
#include <map>

#include "../sdk/math.hh"

struct key_state_t {
    bool is_held;
    bool is_pressed;
    bool is_released;

    key_state_t( ) : is_held( false ), is_pressed( false ), is_released( false ) { };
};

class input_handler {
private:
    std::map< unsigned int, key_state_t > m_key_states{ };

    void update_key( const unsigned int key );
public:
    input_handler( ) = default;

    ~input_handler( ) = default;

    void update_keys( );

    void check_key( const unsigned int key );
    bool is_key_held( const unsigned int key );
    bool is_key_pressed( const unsigned int key );
    bool is_key_released( const unsigned int key );

    static bool is_mouse_in_bounds( const Vector& pos2d, const Vector& sz2d );
    static std::array< int, 2 > get_mouse_pos( );
};

inline input_handler input{ };

#endif //INPUT_HH
