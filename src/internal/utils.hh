#ifndef UTILS_HH
#define UTILS_HH

#include <string>
#include <memory>
#include <map>
#include <ranges>
#include <vector>

#include "log.hh"
#include "../module/module.hh"

namespace utils {
    inline std::map< std::string, std::unique_ptr< module > > m_modules;

    uintptr_t scan_pattern( const std::string& module_name, const std::string& pattern, size_t offset = 0 );
    uintptr_t scan_pattern( const std::string& module_name, const std::vector< byte >& pattern,const std::vector< bool >& mask, size_t pattern_size, size_t offset = 0 );

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

    template < typename T >
    T* bruteforce_interface( const std::string& interface_name, const std::vector< std::string >& blacklisted_modules = { } ) {
        START_LIST( logger::prefixes::info, "waiting for bruteforce", 5, );

        for ( const auto& mod : m_modules | std::views::values ) {
            if ( std::ranges::find( blacklisted_modules, mod->get_filename( ) ) != blacklisted_modules.end( ) )
                continue;

            const auto create_interface = GetProcAddress( mod->get_handle( ), "CreateInterface" );

            if ( !create_interface ) continue;

            const auto inter = reinterpret_cast< T* ( * )( const char*, int* ) >( create_interface )( interface_name.c_str( ), nullptr );

            LOG_LIST( "         \"{}\"->\"{}\"", mod->get_filename( ), interface_name );

            if ( inter ) {
                END_LIST( logger::prefixes::success, "found \"{}\" in \"{}\"", interface_name, mod->get_filename( ) );
                return inter;
            }
        }

        END_LIST( logger::prefixes::error, "failed to find \"{}\"", interface_name );
        return nullptr;
    }

    inline module* get_module( const std::string& module_name ) {
        assert( m_modules.contains( module_name ) );
        return m_modules.at( module_name ).get( );
    };

    template < typename T, typename C >
    T get_vfunc( C* obj, int idx ) {
        // ptr to obj
        // deref -> vtbl (void**)
        // + idx, ptr arithmetic, adds idx * sizeof( void* )
        return reinterpret_cast< T >( *( *reinterpret_cast< void*** >( obj ) + idx ) );
    }

    template < typename T, typename C >
    T get_vftable( C* obj ) {
        return reinterpret_cast< T >( *reinterpret_cast< void*** >( obj ) );
    }
}

#define CREATE_MODULE( module_name ) { \
    const auto module_handle = GetModuleHandleA( module_name ); \
    assert( module_handle ); \
    utils::m_modules.emplace( module_name, std::make_unique< module >( module_handle ) ); \
}



#endif //UTILS_HH
