#ifndef MODULE_HH
#define MODULE_HH
#include "Windows.h"
#include "psapi.h"

#include <cassert>
#include <string>

class module {
private:
     HMODULE m_handle { nullptr };

     std::string m_path;
     std::string m_filename;

     MODULEINFO m_info { };
public:
     // only one initializer
     // module "middleware"-s are created setup
     // in function thread
     explicit module( HMODULE module_handle ) : m_handle( module_handle ) {
          char path_buf[ MAX_PATH ];

          // check note if compile errs
          // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
          GetModuleFileNameA( module_handle, path_buf, MAX_PATH );

          m_path = std::string( path_buf );

          // we can use the same buf to get the filename
          strcpy( path_buf, strrchr( path_buf, '\\' ) );

          // expect to find filename
          assert( path_buf );

          // skip first slash (since srtrrchtrrt returns from last "/...")
          m_filename = std::string( path_buf + 1 );

          printf( "created module \"%s\"\n", m_filename.data( ) );

          const auto curproc = GetCurrentProcess( );
          assert( curproc );

          const auto module_info_res = GetModuleInformation( curproc, module_handle, &m_info, sizeof( m_info ) );
          assert( module_info_res );
     };

     // no deleter because we don't need one now
     ~module( ) = default;

     // pattern is a hex string -> "\xAa\xBb\xCc..."
     // mask is a string of the same size as pattern
     // x's mark normal byte, ?'s mark a wildcard (aka we skip that byte,
     // usually due to it being a pointer => assigned at runtime)
     uintptr_t scan_pattern( const std::string& pattern, const std::string& mask, size_t offset = 0 ) const;

     uintptr_t scan_pattern( const byte* pattern, const byte* mask, size_t pattern_size, size_t offset = 0 ) const;

     size_t get_offset( const uintptr_t from_addr );

     [[nodiscard]] std::string get_filename( ) const {
          return m_filename;
     }

     [[nodiscard]] std::string get_path( ) const {
          return m_path;
     };
};

#endif //MODULE_HH
