#ifndef MODULE_HH
#define MODULE_HH
#include "Windows.h"
#include "psapi.h"

#include <cassert>
#include <string>
#include <vector>

#include "../internal/macros.hh"

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
     explicit module( const HMODULE module_handle ) : m_handle( module_handle ) {
          char path_buf[ MAX_PATH ];

          // check note if compile errs
          // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
          GetModuleFileNameA( module_handle, path_buf, MAX_PATH );

          m_path = std::string( path_buf );

          // we can use the same buf to get the filename
          strcpy_s( path_buf, strrchr( path_buf, '\\' ) );

          // expect to find filename
          assert( path_buf );

          // skip first slash (since srtrrchtrrt returns from last "/...")
          m_filename = std::string( path_buf + 1 );

          LOG_LOADING( "created module \"{}\"", m_filename.data( ) );

          const auto curproc = GetCurrentProcess( );
          assert( curproc );

          const auto module_info_res = GetModuleInformation( curproc, module_handle, &m_info, sizeof( m_info ) );
          assert( module_info_res );
     };

     // no deleter because we don't need one now
     ~module( ) = default;

     // pattern is an ida-style ( "Aa Bb ? Dd ..." ) pattern string (because nullterms were blowing up the hex string)
     // ?'s mark a wildcard (aka we skip that byte, usually due to it being a pointer => assigned at runtime)
     uintptr_t scan_pattern( const std::string& pattern, size_t offset = 0 ) const;

     // string pattern still generates a mask
     // in case we need to use a byte array (never)
     uintptr_t scan_pattern( const std::vector< byte >& pattern, const std::vector< bool >& mask, const size_t pattern_size, const size_t offset = 0 ) const;

     [[nodiscard]] HMODULE get_handle( ) const;

     uintptr_t get_offset_address( size_t );

     size_t get_offset( const uintptr_t from_addr );

     [[nodiscard]] std::string get_filename( ) const {
          return m_filename;
     }

     [[nodiscard]] std::string get_path( ) const {
          return m_path;
     };
};

#endif //MODULE_HH
