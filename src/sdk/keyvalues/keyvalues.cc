#include "keyvalues.hh"

#include "../../internal/utils.hh"

void keyvalues_internals::setup( ) {
    functions::initialize = *reinterpret_cast< void ( __thiscall* )( keyvalues*, const char* ) >(
        utils::scan_pattern( "engine.dll",
                             "55 8B EC 56 8B F1 6A ? FF 75 ? C7 06 ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? FF 15 ? ? ? ? 83 C4 ? 89 06 8B C6" )
    );

    functions::set_int = *reinterpret_cast< void ( __thiscall* )( keyvalues*, const char*, int ) >(
        utils::scan_pattern( "engine.dll",
                             "55 8B EC 6A ? FF 75 ? E8 ? ? ? ? 85 C0 74 ? 8B 4D ? 89 48 ? C6 40 ? ? 5D C2 ? ? CC 55 8B EC 56" )
    );

    functions::set_string = *reinterpret_cast< void ( __thiscall* )(
        keyvalues*, const char*, const char* ) >(
        utils::scan_pattern( "engine.dll", "55 8B EC 57 6A ? FF 75 ? E8 ? ? ? ? 8B F8 85 FF 74 ? 80 7F" )
    );
}

void keyvalues::set_int( const char* name, int value ) {
    keyvalues_internals::functions::set_int( this, name, value );
}

void keyvalues::set_string( const char* name, const char* value ) {
    keyvalues_internals::functions::set_string( this, name, value );
}


