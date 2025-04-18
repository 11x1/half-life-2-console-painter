#include <thread>

#include "Windows.h"
#include "internal/internal.hh"

// https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL, // handle to DLL module
    DWORD fdwReason, // reason for calling function
    LPVOID lpvReserved ) // reserved
{
    printf( "--------\nreason: %d\n--------\n", fdwReason );
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
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}
