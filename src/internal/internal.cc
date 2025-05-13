#include <thread>

#include "internal.hh"

#include "interfaces.hh"
#include "log.hh"
#include "utils.hh"

#include "../hooked/chlclient_framestagenotify.hh"
#include "../hooked/vpanel_paint_traverse.hh"
#include "../hooked/cmatsystem_drawtext.hh"
#include "../hooked/cmodelrender_drawmodelsetup.hh"

#include "../hooks/hooks.hh"

#include "../module/module.hh"

#include "../renderer/surface_wrapper.hh"

#include "../sdk/steamapi/steamutils.hh"
#include "../sdk/client_entitylist.hh"
#include "../sdk/materialsystem.hh"
#include "../sdk/keyvalues/keyvalues.hh"

class steamutils;
using namespace std::chrono_literals;
using namespace std::string_literals;

void internal::setup::main( const HINSTANCE dll_instance ) {
    if ( !GetConsoleWindow( ) ) {
        AllocConsole( );
        freopen_s( reinterpret_cast< FILE ** >( stdout ), "CONOUT$", "w", stdout );
    }

    globals::console_handle = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleMode( globals::console_handle, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING );

    g_log.setup( );

    char win_name_buf[ MAX_PATH ];
    DWORD win_name_len = MAX_PATH;
    GetUserNameA( win_name_buf, &win_name_len );
    LOG( info, "Hello {}, this build is from {}", win_name_buf, __DATE__ );

    std::this_thread::sleep_for( 10s );

    while ( !GetModuleHandleA( "engine.dll" ) )
        std::this_thread::sleep_for( 1s );

    internal::setup::modules( );
    interfaces::setup( );
    surface_wrapper::setup( );
    keyvalues_internals::setup( );

    const auto get_engine_version = utils::scan_pattern( "engine.dll",
                                                         "A1 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC 55 8B EC 8B 45 ? 33 D2" );

    if ( get_engine_version ) {
        using get_engine_version_t = int( __stdcall* )( );
        const auto get_engine_version_func = reinterpret_cast< get_engine_version_t >( get_engine_version );

        LOG( success, "engine version: {}", get_engine_version_func( ) );
    }

    // const auto string_addr = utils::scan_pattern( "engine.dll", ".?AVIVEngineClient013@@", "xxxxxxxxxxxxxxxxxxxxxxx" );
    // printf( "%u\n", string_addr );

    utils::get_vftable( "engine.dll", "CEngineClient" );

    using create_or_find_interface = int( __cdecl* )( int, const char* );
    const auto SteamInternal_FindOrCreateUserInterface = utils::get_proc_address< create_or_find_interface >(
        "steam_api.dll", "SteamInternal_FindOrCreateUserInterface" );

    if ( SteamInternal_FindOrCreateUserInterface ) {
        // printf( "fnaddr: %p\n", SteamInternal_FindOrCreateUserInterface );

        const auto lol = reinterpret_cast< steamutils * >(
            SteamInternal_FindOrCreateUserInterface( 0, "SteamUtils010" ) );

        LOG( success, "batterypc: {}%", static_cast< int >( lol->get_current_battery_power( ) ) );
    }


    if ( interfaces::entitylist ) {
        LOG( debug, "found client entitylist" );

        const auto highest_entity_index = interfaces::entitylist->get_highest_entity_index( );
        LOG( debug, "highest entity index: {}", highest_entity_index );

        int chl2_player_index{ -1 };
        uintptr_t* chl2_player{ nullptr };

        std::map< const char *, size_t > seen_ents{ };

        for ( int i = 0; i < highest_entity_index; ++i ) {
            const auto networked_ent = reinterpret_cast< uintptr_t * >( interfaces::entitylist->
                get_client_networkable( i ) );
            if ( !networked_ent ) continue;

            auto client_class = ( *( int ( __thiscall ** )( uintptr_t* ) ) ( *networked_ent + 8 ) )( networked_ent );
            const auto netname = *( const char ** ) ( client_class + 8 );

            // printf( "%s[%p]\n", netname, networked_ent );

            if ( !seen_ents.contains( netname ) )
                seen_ents[ netname ] = 1;
            else seen_ents[ netname ]++;

            if ( strcmp( netname, "CHL2_Player" ) == 0 && chl2_player_index == -1 ) {
                chl2_player = networked_ent;
                chl2_player_index = i;
            }
        }

        LOG( info, "entdump" );
        for ( const auto pair : seen_ents ) {
            LOG( info, "         {} {} entitie(s)", pair.second, pair.first );
        }

        if ( chl2_player ) {
            LOG( debug, "CHL2_Player addr: {:p}", reinterpret_cast< void* >( chl2_player ) );
            const auto cliententity = reinterpret_cast< byte * >( interfaces::entitylist->get_client_entity(
                chl2_player_index ) );
            LOG( debug, "same as IClientEntity: {:p}", reinterpret_cast< void* >( chl2_player ) );

            // 28 D0 96 5A
            // CC D4 96 5A
            // 84 D5 96 5A
            // BC D5 96 5A
            // first 16 bytes are 4 vftables
            // (found via cheatengine)
            // dump 'em!

            const auto mod = utils::get_module( "client.dll" );

            for ( int j = 0; j < 4; j++ ) {
                const auto vftbl = *reinterpret_cast< uintptr_t * >( cliententity + j * 4 );
                const auto off = mod->get_offset( vftbl );
                LOG( debug, "         vftbl {} offset= 0x{:X}", j, off );
            }
        }
    } else {
        LOG( error, "couldn't find client entitylist" );
    }

    INITIALIZE_VFTABLE_HOOK( engine.dll, CClientState, chlclient_framestagenotify );
    INITIALIZE_VFTABLE_HOOK( vgui2.dll, VPanelWrapper, vpanel_paint_traverse );
    INITIALIZE_VFTABLE_HOOK( engine.dll, CModelRender, cmodelrender_drawmodelsetup );
    INITIALIZE_VFTABLE_HOOK( vguimatsurface.dll, CMatSystemSurface, cmatsystem_drawtext );

    const auto matsystem = utils::bruteforce_interface< materialsystem >( "VMaterialSystem080" );

    // todo: material builder for chams (needs menu framework)
    //       https://developer.valvesoftware.com/wiki/Category:Shader_parameters

    if ( matsystem ) {
        LOG( debug, "creating testmaterial" );

        auto test_mat = keyvalues( "UnlitGeneric" );
        test_mat.set_string( "$basetexture", "vgui/white_additive" );
        test_mat.set_int( "$ignorez", 1 );
        test_mat.set_int( "$translucent", 1 );
        test_mat.set_int( "$nocull", 1 );

        mats::test_material = matsystem->create_material( "frank_test", &test_mat );

        const auto cmodelrender_vftable = utils::get_vftable( "engine.dll", "CModelRender" );
        modelrender_internals::forced_material_override = *reinterpret_cast< void(__stdcall*)(material*, int) >( reinterpret_cast< void** >( cmodelrender_vftable )[ 1 ] );
    } else {
        LOG( error, "failed to find VMaterialSystem081" );
    }

    LOG( info, "press END to unninject" );

    while ( !GetAsyncKeyState( VK_END ) )
        std::this_thread::sleep_for( 500ms );

    hooks::unhook_all( );

    LOG( info, "bye" );

    fclose( stdout );
    FreeConsole( );
    FreeLibraryAndExitThread( dll_instance, EXIT_SUCCESS );
}

void internal::setup::modules( ) {
    // CREATE_MODULE( "steam_api.dll" );
    //
    // CREATE_MODULE( "engine.dll" );
    // CREATE_MODULE( "client.dll" );
    // CREATE_MODULE( "server.dll" );

    // go through all modules in app
    // and add them to the list
    HMODULE hMods[ 1024 ];
    HANDLE hProcess = GetCurrentProcess( );
    DWORD cbNeeded;

    const auto modules_list = g_log.list( 10 )->prefix( "         loaded module " )->spew( );
    const auto line_waiting_modules = g_log.line( "loading modules" )->prefix( log::components::prefix( "loading", color( 0xffe699 ) ) )->spew( );

    if ( EnumProcessModules( hProcess, hMods, sizeof( hMods ), &cbNeeded ) ) {
        for ( unsigned int i = 0; i < ( cbNeeded / sizeof( HMODULE ) ); i++ ) {
            char szModName[ MAX_PATH ];
            GetModuleBaseNameA( hProcess, hMods[ i ], szModName, sizeof( szModName ) );
            CREATE_MODULE( std::string( szModName ).c_str( ) );

            modules_list->line( std::string( szModName ) );
        }
    }

    line_waiting_modules->update_entry( *log::components::entry_t( std::format( "loaded {} modules", cbNeeded / sizeof( HMODULE ) ) ).prefix( log::prefixes::success ) );
}
