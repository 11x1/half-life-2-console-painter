#include <thread>

#include "internal.hh"

#include "interfaces.hh"
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
    interfaces::setup(  );
    surface_wrapper::setup( );

    const auto get_engine_version = utils::scan_pattern( "engine.dll", "A1 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC 55 8B EC 8B 45 ? 33 D2" );

    if ( get_engine_version ) {
        using get_engine_version_t = int( __stdcall* )( );
        const auto get_engine_version_func = reinterpret_cast< get_engine_version_t >( get_engine_version );

        printf( "Engine version: %d\n", get_engine_version_func( ) );
    }

    // const auto string_addr = utils::scan_pattern( "engine.dll", ".?AVIVEngineClient013@@", "xxxxxxxxxxxxxxxxxxxxxxx" );
    // printf( "%u\n", string_addr );

    utils::get_vftable( "engine.dll", "CEngineClient" );

    using create_or_find_interface = int( __cdecl* )( int, const char* );
    const auto SteamInternal_FindOrCreateUserInterface = utils::get_proc_address< create_or_find_interface >( "steam_api.dll", "SteamInternal_FindOrCreateUserInterface" );

    if ( SteamInternal_FindOrCreateUserInterface ) {
        printf( "fnaddr: %p\n", SteamInternal_FindOrCreateUserInterface );

        const auto lol = reinterpret_cast< steamutils* >( SteamInternal_FindOrCreateUserInterface( 0, "SteamUtils010" ) );

        printf( "batterypc: %d%%\n", lol->get_current_battery_power(  ) );
    }


    if ( interfaces::entitylist ) {
        printf( "found client entitylist\n" );

        const auto highest_entity_index = interfaces::entitylist->get_highest_entity_index( );
        printf( "highest entity index: %d\n", highest_entity_index );

        int chl2_player_index { -1 };
        uintptr_t* chl2_player { nullptr };

        std::map< const char*, size_t > seen_ents { };

        for ( int i = 0; i < highest_entity_index; ++i ) {
            const auto networked_ent = reinterpret_cast< uintptr_t* >( interfaces::entitylist->get_client_networkable( i ) );
            if ( !networked_ent ) continue;

            auto client_class = (*(int (__thiscall **)(uintptr_t *))(*networked_ent + 8))(networked_ent);
            const auto netname = *(const char **)(client_class + 8);

            printf( "%s[%p]\n", netname, networked_ent );

            if ( !seen_ents.contains( netname ) )
                seen_ents[ netname ] = 1;
            else seen_ents[ netname ]++;

            if ( strcmp( netname, "CHL2_Player" ) == 0 && chl2_player_index == -1 ) {
                chl2_player = networked_ent;
                chl2_player_index = i;
            }
        }

        printf( "entdump\n\n" );
        for ( const auto pair : seen_ents ) {
            printf( "\t%d %s entities\n", pair.second, pair.first );
        }
        printf( "\n\n" );

        if ( chl2_player ) {
            printf( "CHL2_Player addr: %p\n", chl2_player );
            const auto cliententity = reinterpret_cast< byte* >( interfaces::entitylist->get_client_entity( chl2_player_index ) );
            printf( "same as IClientEntity: %p\n", cliententity );

            // 28 D0 96 5A
            // CC D4 96 5A
            // 84 D5 96 5A
            // BC D5 96 5A
            // first 16 bytes are 4 vftables
            // (found via cheatengine)
            // dump 'em!

            const auto mod = utils::get_module( "client.dll" );

            for ( int j = 0; j < 4; j++ ) {
                const auto vftbl = *reinterpret_cast< uintptr_t* >( cliententity + j * 4 );
                const auto off = mod->get_offset( vftbl );
                printf( "\tvftbl %d offset= %#x\n", j, off );
            }
        }
    } else {
        printf( "Couldn't find client entitylist\n" );
    }


    /*
    // const auto chlclient = utils::scan_pattern( "engine.dll", "\x8B\x0D\x00\x00\x00\x00\x83\xEC\x00\x85\xC9\x74\x00\x8B\x15", "xx????xx?xxx?xx" );
    const auto chlclient_vftable = utils::get_vftable( "client.dll", "CHLClient" );

    auto chlclient_vmt_hook = hooks::make_vmt_hook( chlclient_vftable );

    const bool succ = chlclient_vmt_hook.hook< chlclient_hook::def >( chlclient_hook::index, chlclient_hook::hook );

    if ( succ )
        chlclient_hook::original = chlclient_vmt_hook.get_original< chlclient_hook::def >( chlclient_hook::index );
    */

    const auto vgui_panel_wrapper_vftable = utils::get_vftable( "vgui2.dll", "VPanelWrapper" );
    auto vgui_panel_wrapper_vmt_hook = hooks::make_vmt_hook( vgui_panel_wrapper_vftable );

    const bool succ_vgui = vgui_panel_wrapper_vmt_hook.hook< vpanel_paint_traverse::def >( vpanel_paint_traverse::index, vpanel_paint_traverse::hook );

    if ( succ_vgui )
        vpanel_paint_traverse::original = vgui_panel_wrapper_vmt_hook.get_original< vpanel_paint_traverse::def >( vpanel_paint_traverse::index );

    const auto imatsystem_surface_vftable = utils::get_vftable( "vguimatsurface.dll", "CMatSystemSurface" );
    auto imatsystem_surface_vmt_hook = hooks::make_vmt_hook( imatsystem_surface_vftable );
    const bool succ_imatsystem_surface = imatsystem_surface_vmt_hook.hook< cmatsystem_drawtext::def >( cmatsystem_drawtext::index, cmatsystem_drawtext::hook );

    if ( succ_imatsystem_surface )
        cmatsystem_drawtext::original = imatsystem_surface_vmt_hook.get_original< cmatsystem_drawtext::def >( cmatsystem_drawtext::index );

    const auto cmodelrender_vftable = utils::get_vftable( "engine.dll", "CModelRender" );
    auto cmodelrender_vmt_hook = hooks::make_vmt_hook( cmodelrender_vftable );
    const bool succ_cmodelrender = cmodelrender_vmt_hook.hook< cmodelrender_drawmodelsetup::def >( cmodelrender_drawmodelsetup::index, cmodelrender_drawmodelsetup::hook );

    if ( succ_cmodelrender )
        cmodelrender_drawmodelsetup::original = cmodelrender_vmt_hook.get_original< cmodelrender_drawmodelsetup::def >( cmodelrender_drawmodelsetup::index );

    printf( "waiting for end\n" );

    while ( !GetAsyncKeyState( VK_END ) )
        std::this_thread::sleep_for( 500ms );

    vgui_panel_wrapper_vmt_hook.unhook( vpanel_paint_traverse::index );
    imatsystem_surface_vmt_hook.unhook( cmatsystem_drawtext::index );
    cmodelrender_vmt_hook.unhook( cmodelrender_drawmodelsetup::index );

    printf( "bye\n" );

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

    if ( EnumProcessModules( hProcess, hMods, sizeof( hMods ), &cbNeeded ) ) {
        for ( unsigned int i = 0; i < ( cbNeeded / sizeof( HMODULE ) ); i++ ) {
            char szModName[ MAX_PATH ];
            GetModuleBaseNameA( hProcess, hMods[ i ], szModName, sizeof( szModName ) );
            CREATE_MODULE( std::string( szModName ).c_str( ) );
        }
    }
}
