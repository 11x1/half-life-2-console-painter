#include <thread>

#include "Windows.h"
#include "internal/internal.hh"
#include "internal/log.hh"

#define MAKE_CASE( enm, name, var ) \
    case enm: { \
        var = #name; \
        break; \
    }

// https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL, // handle to DLL module
    DWORD fdwReason, // reason for calling function
    LPVOID lpvReserved ) // reserved
{
    // Perform actions based on the reason for calling.
    switch ( fdwReason ) {
        case DLL_PROCESS_ATTACH:
            // disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH
            DisableThreadLibraryCalls( hinstDLL );

            // execute within the virtual address space of the calling process
            CreateThread( nullptr, 0,
                          ( LPTHREAD_START_ROUTINE ) internal::setup::main,
                          hinstDLL, 0, nullptr );
        //printf( "ATTACH\n" );
            break;
        case DLL_PROCESS_DETACH:

            if ( lpvReserved != nullptr ) {
                break; // do not do cleanup if process termination scenario
            }
            break;
    }

    if ( GetStdHandle( STD_OUTPUT_HANDLE ) != INVALID_HANDLE_VALUE ) {
        // HMODULE and HINSTANCE are the same today, but represented different things in 16-bit Windows.
        // https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
        char path[ MAX_PATH ];
        DWORD path_len = MAX_PATH;
        GetModuleFileNameA( hinstDLL, path, path_len );

        const auto filename = std::string( strrchr( path, '\\' ) + 1 );

        std::string reason;
        switch ( fdwReason ) {
            MAKE_CASE( DLL_PROCESS_ATTACH, ATTACHED, reason );
            MAKE_CASE( DLL_PROCESS_DETACH, DETACHED, reason );
            MAKE_CASE( DLL_THREAD_ATTACH, ATTACHED (T), reason );
            MAKE_CASE( DLL_THREAD_DETACH, DETACHED (T), reason );
            default: reason = "UNKNOWN";
        }

        std::string mode;

        if ( fdwReason == DLL_PROCESS_ATTACH ) {
            mode = lpvReserved == nullptr ? "dynamic" : "static";
        } else if ( fdwReason == DLL_PROCESS_DETACH ) {
            mode = lpvReserved == nullptr ? "freelibrary/failed" : "process terminating";
        } else {
            mode = std::format( "unknown[{:p}]", lpvReserved );
        }

        LOG( warning, "{} module \"{}\" ({})", reason, filename, mode );
    }

    return TRUE; // Successful DLL_PROCESS_ATTACH.
}
