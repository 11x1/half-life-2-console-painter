//
// Created by riis on 4/16/2025.
//

#ifndef MODULE_HH
#define MODULE_HH
#include "Windows.h"
#include <Psapi.h>

#include <cassert>
#include <string>

class module {
private:
     HMODULE m_handle { nullptr };

     std::string m_path;
     std::string m_filename;
public:
     // only one initializer
     // module "middleware"-s are created setup
     // in function thread
     explicit module( HANDLE proc_handle, HMODULE module_handle ) : m_handle( module_handle ) {
          char path_buf[ MAX_PATH ];

          // check note if compile errs
          // https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getmodulefilenameexa
          GetModuleFileNameExA( proc_handle, module_handle, path_buf, MAX_PATH );

          m_path = std::string( path_buf );

          // we can use the same buf to get the filename
          strcpy( path_buf, strrchr( path_buf, '\\' ) );

          // expect to find filename
          assert( path_buf );

          // skip first slash (since srtrrchtrrt returns from last "/...")
          m_filename = std::string( path_buf + 1 );
     };

     // no deleter because we don't need one now
     ~module( ) = default;

     [[nodiscard]] std::string get_filename( ) const {
          return m_filename;
     }

     [[nodiscard]] std::string get_path( ) const {
          return m_path;
     };
};

#endif //MODULE_HH
