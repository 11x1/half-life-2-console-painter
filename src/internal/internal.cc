#include <thread>

#include "internal.hh"

#include "../hooked/chlclient_framestagenotify.hh"
#include "../hooks/hooks.hh"
#include "../module/module.hh"
#include "../sdk/steamapi/steamutils.hh"
#include "../sdk/client_entitylist.hh"

#define CREATE_MODULE( module_name ) { \
    const auto module_handle = GetModuleHandleA( module_name ); \
    assert( module_handle ); \
    internal::m_modules.emplace( module_name, std::make_unique< module >( module_handle ) ); \
}

class steamutils;
using namespace std::chrono_literals;
using namespace std::string_literals;

void internal::setup::main( HINSTANCE dll_instance ) {
    AllocConsole( );
    freopen_s( reinterpret_cast< FILE** >( stdout ), "CONOUT$", "w", stdout );

    const auto console_handle = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleMode( console_handle, ENABLE_PROCESSED_OUTPUT );

    while ( !GetModuleHandleA( "engine.dll" ) )
        std::this_thread::sleep_for( 1s );

    internal::setup::modules( );

    const auto get_engine_version = utils::scan_pattern( "engine.dll", "\xA1\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x8B\x45\x00\x33\xD2"s,  "x????xxxxxxxxxxxxxxxx?xx"s );

    if ( get_engine_version ) {
        using get_engine_version_t = int( __stdcall* )( );
        const auto get_engine_version_func = reinterpret_cast< get_engine_version_t >( get_engine_version );

        printf( "Engine version: %d\n", get_engine_version_func( ) );
    }

    // const auto string_addr = utils::scan_pattern( "engine.dll", ".?AVIVEngineClient013@@", "xxxxxxxxxxxxxxxxxxxxxxx" );
    // printf( "%u\n", string_addr );

    utils::get_vftable( "engine.dll", "CEngineClient" );

    /*
    const auto chlclient = utils::scan_pattern( "engine.dll", "\x8B\x0D\x00\x00\x00\x00\x83\xEC\x00\x85\xC9\x74\x00\x8B\x15", "xx????xx?xxx?xx" );
    const auto chlclient_vftable = utils::get_vftable( "client.dll", "CHLClient" );

    const auto chlclient_vmt_hook = hooks::make_vmt_hook( chlclient_vftable );

    const bool succ = chlclient_vmt_hook->hook< chlclient_hook::def >( chlclient_hook::index, chlclient_hook::hook );

    if ( succ )
        chlclient_hook::original = chlclient_vmt_hook->get_original< chlclient_hook::def >( chlclient_hook::index );
    */

    using create_or_find_interface = int( __cdecl* )( int, const char* );
    const auto SteamInternal_FindOrCreateUserInterface = utils::get_proc_address< create_or_find_interface >( "steam_api.dll", "SteamInternal_FindOrCreateUserInterface" );

    if ( SteamInternal_FindOrCreateUserInterface ) {
        printf( "fnaddr: %p\n", SteamInternal_FindOrCreateUserInterface );

        const auto lol = reinterpret_cast< steamutils* >( SteamInternal_FindOrCreateUserInterface( 0, "SteamUtils010" ) );

        printf( "batterypc: %d%%\n", lol->get_current_battery_power(  ) );
    }


    printf( "getting entlist\n" );
    const auto entitylist = utils::create_interface< client_entitylist >( "client.dll", "VClientEntityList003" );
    printf( "done.\n" );

    if ( entitylist ) {
        printf( "found client entitylist\n" );

        const auto highest_entity_index = entitylist->get_highest_entity_index( );
        printf( "highest entity index: %d\n", highest_entity_index );

        for ( int i = 0; i < highest_entity_index; ++i ) {
            const auto networked_ent = reinterpret_cast< uintptr_t* >( entitylist->get_client_networkable( i ) );
            if ( !networked_ent ) continue;

            printf( "entity: %d %p\n", i, networked_ent );

            auto client_class = (*(int (__thiscall **)(uintptr_t *))(*networked_ent + 8))(networked_ent);
            printf( "m_pNetworkName: %s\n", *(const char **)(client_class + 8) );
        }
    } else {
        printf( "Couldn't find client entitylist\n" );
    }

    printf( "waiting for end\n" );

    while ( !GetAsyncKeyState( VK_END ) )
        std::this_thread::sleep_for( 500ms );

    // chlclient_vmt_hook->unhook_all( );

    printf( "bye\n" );

    FreeConsole( );
    FreeLibraryAndExitThread( dll_instance, EXIT_SUCCESS );
}

void internal::setup::modules( ) {
    CREATE_MODULE( "steam_api.dll" );

    CREATE_MODULE( "engine.dll" );
    CREATE_MODULE( "client.dll" );
    CREATE_MODULE( "server.dll" );
}

uintptr_t utils::scan_pattern( const std::string& module_name, const std::string& pattern, const std::string& mask,
    size_t offset ) {
    // expect to have the module defined already
    assert( m_modules.contains( module_name ) );

    const auto mod = m_modules.at( module_name ).get( );
    return mod->scan_pattern( pattern, mask, offset );
}

uintptr_t utils::scan_pattern( const std::string& module_name, const byte* pattern, const byte* mask, size_t pattern_size, size_t offset ) {
    // expect to have the module defined already
    assert( m_modules.contains( module_name ) );

    const auto mod = m_modules.at( module_name ).get( );
    return mod->scan_pattern( pattern, mask, pattern_size, offset );
}

uintptr_t utils::get_vftable( const std::string& module_name, const std::string& class_name ) {
    const auto debug_mod = m_modules[ module_name ].get( );

    const std::string rtti_class_name = std::format( ".?AV{}@@", class_name );
    printf( "rtti_class_name: %s\n", rtti_class_name.c_str( ) );;

    const auto mask = std::string( rtti_class_name.size( ), 'x' );
    // could only search in .data section, maybe some other day
    const auto str_addr = utils::scan_pattern( module_name, rtti_class_name, mask );

    if ( !str_addr ) return 0;

    printf( "straddr: %#x\n", debug_mod->get_offset( str_addr ) );

    // type info (32bit) is type descriptor name address
    // - 8 bytes
    // open up a type descriptor name in ida
    // the layout is something similiar
    //
    // Aa Bb Cc Dd       - type info address
    // 00 00 00 00       - rtti pad or whatever
    // 2E 3F 41 56 ...   - type descriptor name

    // fun fact, the mangled name for type descriptor
    // addresses is ??_R0?AV{classname}@@@8

    // now this cool type info is addressed by col
    // and col is referenced by the vftable (basically class method table)
    // for more info: https://blog.rop.la/en/reversing/2022/12/13/identifying-vftables-through-ms-cpp-rtti.html

    // @ 0xAaBbCcDd
    const auto type_info_addr = str_addr - 0x8;
    printf( "type_info_addr: %#x\n", debug_mod->get_offset( type_info_addr ) );

    // cool thing about col (aka Complete Object Locator) is that
    // it has a recognisable signature (also references type info inside of it as said before)
    // the col structure is something like this
    //
    // 00 00 00 00   - signature
    // ?? ?? ?? ??   - offset
    // ?? ?? ?? ??   - offset to base class, if inherited
    // Dd Cc Bb Aa   - ptr to type info (le)
    //
    // since there might be many matching patterns
    // for the wanted col, we want to prioritise
    // the lowest offset + inheritance combo

    const byte col_mask[ 16 ] {
        'x', 'x', 'x', 'x',
        '?', '?', '?', '?',
        '?', '?', '?', '?',
        'x', 'x', 'x', 'x'
    };

    byte col_pattern[ 16 ] {
        0, 0, 0, 0,
        0, 0, 0, 0, // this
        0, 0, 0, 0, // and this r skipped
        static_cast<byte>( type_info_addr       & 0xFF ), // Dd (lsb)
        static_cast<byte>( type_info_addr >>  8 & 0xFF ), // Cc
        static_cast<byte>( type_info_addr >> 16 & 0xFF ), // Bb
        static_cast<byte>( type_info_addr >> 24 & 0xFF ), // Aa
    };

    printf( "colpat:\n12*(0 0 0 0) + %x %x %x %x\n", col_pattern[ 12 ], col_pattern[ 13 ], col_pattern[ 14 ], col_pattern[ 15 ] );

    size_t off { 0 };
    uintptr_t last_vftbl { 0 };
    while ( const auto a = utils::scan_pattern( module_name, col_pattern, col_mask, 16, off ) ) {
        off = m_modules[ module_name ].get( )->get_offset( a ) + 16;

        printf( "Found col pattern match at %#x\n", m_modules[ module_name ].get( )->get_offset( a ) );

        // find a ptr to col addr
        byte col_ptr_pat[ 4 ]{
            static_cast<byte>( a       & 0xFF ), // Dd (lsb)
            static_cast<byte>( a >>  8 & 0xFF ), // Cc
            static_cast<byte>( a >> 16 & 0xFF ), // Bb
            static_cast<byte>( a >> 24 & 0xFF ), // Aa
        };

        byte col_ptr_mask[ 4 ]{ 'x', 'x', 'x', 'x' };
        const auto not_col = utils::scan_pattern( module_name, col_ptr_pat, col_ptr_mask, 4 );

        printf( "col ref @ %#x (off=%#x) -> vftbl: %#x (off=%#x)\n", not_col, debug_mod->get_offset( not_col ), not_col + 0x4, debug_mod->get_offset( not_col + 0x4 ) );
        last_vftbl = not_col + 0x4;
    };

    return last_vftbl;
}
