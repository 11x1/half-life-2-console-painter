#ifndef KEYVALUES_HH
#define KEYVALUES_HH
#include <cassert>
#include <string>

// forwarddecl for internals
class keyvalues;

namespace keyvalues_internals {
    namespace functions {
        inline void (__thiscall* initialize)( keyvalues*, const char* );
        inline void (__thiscall* set_int)( keyvalues*, const char*, int );
        inline void (__thiscall* set_string)( keyvalues*, const char*, const char* );
    }

    void setup( );
}

/*
- size = 32 bytes
-- ...
v9 = sub_10276DF0(32); // size
if ( v9 )
  v10 = new_keyvalues(v9, "UnlitGeneric"); // not sure if this is the correct decomp
                                           // but it's returning this anyway so should
                                           // be ok
else
  v10 = 0;
KeyValues::SetString(v10, "$basetexture", Src);
KeyValues::SetInt(v10, "$ignorez", 1);
-- ...
*/
class keyvalues {
private:
    int pad0;
    int pad4;
    int pad8;
    int m_iValue;
    char m_type;
    int pad14;
    int pad18;
    int pad1C;

public:
    explicit keyvalues( const std::string& name )
    // clion shut up
        : pad0( 0 ), pad4( 0 ), pad8( 0 ), m_iValue( 0 ), m_type( 0 ), pad14( 0 ), pad18( 0 ), pad1C( 0 ) {
        // internals need to be initialized before using, done in setup
        assert( keyvalues_internals::functions::initialize );
        keyvalues_internals::functions::initialize( this, name.c_str( ) );
    }

    void set_int( const char* name, int value );
    void set_string( const char* name, const char* value );
};


#endif //KEYVALUES_HH
