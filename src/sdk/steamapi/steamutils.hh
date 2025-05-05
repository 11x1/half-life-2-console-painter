#ifndef STEAMUTILS_HH
#define STEAMUTILS_HH

class steamutils {
public:
    steamutils( ) = default;
    ~steamutils( ) = default;

    char get_current_battery_power( ) {
        return reinterpret_cast< char( __thiscall* )( steamutils* ) >( *( *reinterpret_cast< void*** >( this ) + 0x20 / 4 ) )( this );
    }
};

#endif //STEAMUTILS_HH
