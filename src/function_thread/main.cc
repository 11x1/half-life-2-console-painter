#include "main.hh"

#include <Windows.h>
#include <Psapi.h>

#include <iostream>
#include <vector>

#include "../globals.hh"
#include "../module/module.hh"
#include "../state_manager/state_manager.hh"

inline std::vector< const char* > IGNORED_WINDOWS {
    "MSCTFIME UI",
    "Default IME"
};

inline bool is_ignored( const char* what ) {
    for ( const auto& wnd_name : IGNORED_WINDOWS ) {
        if ( strcmp( what, wnd_name ) == 0 )
            return true;
    }

    return false;
}

static BOOL CALLBACK enum_cb( HWND hwnd, LPARAM lparam ) {
    if ( hwnd ) {
        const char* buf = new char[ 256 ];
        GetWindowTextA( hwnd, (LPSTR)buf, 256 );

        if ( strlen( buf ) > 0 && !is_ignored( buf ) )
            printf( "%p %s\n", hwnd, buf );

        delete buf;
    }

    return true;
}

void function_thread::main( ) {
    // find window handle
    EnumWindows( enum_cb, 0 );

    while ( m_game_hwnd == nullptr ) {
        m_game_hwnd = FindWindowW( nullptr, L"HALF-LIFE 2 - Direct3D 9" );
        g_state_manager.set_state( WAITING_FOR_WINDOW );
        Sleep( 1000 );
    }

    g_state_manager.set_state( FOUND_WINDOW );

    // we can setup everything ingame with the window handle
    auto did_setup = this->setup( );

    if ( !did_setup )
        return;

    while ( g_should_run )
        Sleep( 1000 );

    std::cout << "function_thread::main() exiting" << std::endl;
}

bool function_thread::setup( ) {
    assert( m_game_hwnd );

    DWORD proc_id { 0 };
    GetWindowThreadProcessId( m_game_hwnd, &proc_id );

    if ( !proc_id ) {
        g_state_manager.set_state( FAILED );
        printf( "failed to get procid\n" );
        return false;
    }

    HANDLE proc_handle = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, proc_id );
    if ( !proc_handle ) {
        g_state_manager.set_state( FAILED );
        printf( "failed to get prochandle\n" );
        return false;
    }

    // https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-modules-for-a-process
    HMODULE modules[ 256 ];
    DWORD bytes_needed;
    if ( EnumProcessModules( proc_handle, modules, sizeof( modules ), &bytes_needed ) ) {
        g_state_manager.set_state( FOUND_MODULES );

        size_t module_count = bytes_needed / sizeof( HMODULE );

        for ( size_t i = 0; i < module_count; i++ )
            m_modules.push_back( std::make_unique< module >( proc_handle, modules[ i ] ) );
    } else {
        g_state_manager.set_state( FAILED );
        printf( "failed to enum procmodules" );
        return false;
    }

    printf( "found %d modules\n", m_modules.size( ) );
    for ( const auto& mod : m_modules ) {
        printf( "%s ", mod->get_filename( ).data( ) );
    }
    printf( "\n" );

    return true;
}

void function_thread::render( ImDrawList& draw_list ) {

}
