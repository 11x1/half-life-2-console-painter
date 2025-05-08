#ifndef CLIENT_ENTITYLIST_HH
#define CLIENT_ENTITYLIST_HH
#include <cstdint>

#include "../internal/utils.hh"

class client_entitylist {
public:
    client_entitylist( ) = default;
    ~client_entitylist( ) = default;

    uintptr_t get_client_networkable( int idx ) {
        return utils::get_vfunc< uintptr_t( __thiscall* )( client_entitylist*, int ) >( this, 0 )( this, idx );
    }

    uintptr_t get_client_entity( int idx ) {
        return reinterpret_cast< uintptr_t( __thiscall* )( client_entitylist*, int ) >( *( *reinterpret_cast< void*** >( this ) + 0x0C / 4 ) )( this, idx );
    }

    int get_highest_entity_index( ) {
        return reinterpret_cast< int( __thiscall* )( client_entitylist* ) >( *( *reinterpret_cast< void*** >( this ) + 0x18 / 4 ) )( this );
    }
};

#endif //CLIENT_ENTITYLIST_HH
