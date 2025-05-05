#ifndef INTERNAL_HH
#define INTERNAL_HH

#include <map>
#include "../module/module.hh"

inline namespace internal {
    inline std::map< std::string, std::unique_ptr< module > > m_modules;

    namespace setup {
        void main( HINSTANCE dll_instance );

        void modules( );
    }

    namespace utils {
        uintptr_t scan_pattern( const std::string& module_name, const std::string& pattern, const std::string& mask, size_t offset = 0 );
        uintptr_t scan_pattern( const std::string& module_name, const byte* pattern, const byte* mask, size_t pattern_size, size_t offset = 0 );

        template< typename T >
        T get_proc_address( const std::string& module_name, const std::string& proc_name ) {
            assert( m_modules.contains( module_name ) );
            const auto mod = m_modules.at( module_name ).get( );
            const auto proc_addr = GetProcAddress( mod->get_handle( ), proc_name.c_str( ) );

            return reinterpret_cast< T >( proc_addr );
        };

        uintptr_t get_vftable( const std::string& module_name, const std::string& class_name );

        template < typename T >
        T* create_interface( const std::string& module_name, const std::string& interface_name ) {
            assert( m_modules.contains( module_name ) );
            const auto mod = m_modules.at( module_name ).get( );

            const auto create_interface = GetProcAddress( mod->get_handle( ), "CreateInterface" );
            assert( create_interface );

            return reinterpret_cast< T* ( * )( const char*, int* ) >( create_interface )( interface_name.c_str( ), nullptr );
        };
    }
};



#endif //INTERNAL_HH
