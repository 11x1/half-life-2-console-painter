#include "Windows.h"
#include "tlhelp32.h"

#include <filesystem>

#include "loader.hh"

#define error( msg ) { \
    m_last_err = msg; \
    return false; \
}

bool loader::inject( char* dll_path, const char* target_process_name ) {
    const bool file_exists = std::filesystem::exists( dll_path );

    if ( !file_exists )
        error( "file does not exist" );

    // https://stackoverflow.com/a/55030118
    // https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
    // get all processes, including heaps, modules and threads
    PROCESSENTRY32 pi;
    pi.dwSize = sizeof( PROCESSENTRY32 );

    HANDLE processes = CreateToolhelp32Snapshot( TH32CS_SNAPALL, 0 );

    if ( processes == INVALID_HANDLE_VALUE )
        error( "could not create snapshot" );

    Process32First( processes, &pi );
    while ( Process32Next( processes, &pi ) )
        if ( strcmp( pi.szExeFile, target_process_name ) == 0 )
            break;

    CloseHandle( processes );

    // get procid
    HANDLE h_process = OpenProcess( PROCESS_ALL_ACCESS, false, pi.th32ProcessID );

    if ( !h_process )
        error( "failed to open process" );


    // alloc memory for dll path
    // https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex
    // "If lpAddress is NULL, the function determines where to allocate the region."
    LPVOID alloced_addr = VirtualAllocEx( h_process, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );

    if ( !alloced_addr ) {
        CloseHandle( h_process );
        error( "failed to allocate memory in tgt process" );
    }

    // write dll path to memory
    // virtualalloc made space for it in target
    // https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
    if ( !WriteProcessMemory( h_process, alloced_addr, dll_path, strlen( dll_path ), nullptr ) ) {
        // wpm failed
        VirtualFreeEx( h_process, alloced_addr, 0, MEM_RELEASE );
        CloseHandle( h_process );
        error( "failed to write memory in tgt process" );
    }

    // find kernel32 module
    // we can load the dll with LoadLibraryA from kernel32's exports
    // todo: should we check if kernel32 & loadlib exist?
    HMODULE k32 = GetModuleHandleA( "kernel32.dll" );
    LPTHREAD_START_ROUTINE load_library_a_addr = reinterpret_cast< LPTHREAD_START_ROUTINE >( GetProcAddress( k32, "LoadLibraryA" ) );

    // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
    // run in the target process
    HANDLE h_thread = CreateRemoteThread( h_process, nullptr, 0,
                                          load_library_a_addr,
                                          alloced_addr, 0, nullptr );

    if ( !h_thread ) {
        VirtualFreeEx( h_process, alloced_addr, 0, MEM_RELEASE );
        CloseHandle( h_process );
        error( "failed to create remote thread" );
    }

    // wait until loadlibrary is done
    WaitForSingleObject( h_thread, INFINITE );

    // close & free everything
    CloseHandle( h_thread );
    VirtualFreeEx( h_process, alloced_addr, 0, MEM_RELEASE );
    CloseHandle( h_process );

    return true;
}
