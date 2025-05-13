#ifndef VMT_HOOK_HH
#define VMT_HOOK_HH
#include <algorithm>

#include "Windows.h"

#include <cassert>
#include <cstdint>
#include <map>
#include <ranges>
#include <utility>
#include <vector>


class vmt_hook {
private:
    std::string m_name{ "<empty hook name>" };
    uintptr_t m_vtbl_addr;

    // we set the original at index to map
    // when we hook sm
    std::map< size_t, uintptr_t > m_originals{ };

public:
    explicit vmt_hook( const uintptr_t vtbl_addr ) : m_vtbl_addr( vtbl_addr ) {
    }

    explicit vmt_hook( const uintptr_t vtbl_addr, std::string name ) : m_name( std::move( name ) ),
                                                                       m_vtbl_addr( vtbl_addr ) {
    }

    ~vmt_hook( ) { unhook_all( ); }

    template< typename Fn >
    bool hook( size_t index, Fn override_fn ) {
        LOG_LOADING( "\"{}\": step 0", m_name.c_str( ) );

        // dont hook twice, otherwise we lose the original
        if ( m_originals.contains( index ) )
            return false;

        LOG_LOADING( "\"{}\": step 1", m_name.c_str( ) );

        auto* vf_arr = reinterpret_cast< uintptr_t * >( m_vtbl_addr );

        DWORD old_prot;
        bool succ = VirtualProtect(
            // cant index, otherwise we're not
            // writing to the function address
            reinterpret_cast< void * >( m_vtbl_addr + index * sizeof( void * ) ),
            sizeof( void * ),
            PAGE_EXECUTE_READWRITE,
            &old_prot
        );

        LOG_LOADING( "VirtualProtect({:p}, {}, {}, {})\n",
                reinterpret_cast< void* >( m_vtbl_addr + index * sizeof( void * ) ),
                sizeof( void * ),
                PAGE_EXECUTE_READWRITE,
                old_prot
        );

        if ( !succ ) {
            LOG_ERROR( "\"{}\"failed to protect memory: {}", m_name.c_str( ), GetLastError( ) );
            return false;
        }

        LOG_LOADING( "\"{}\": step 2", m_name.c_str( ) );

        // store original function, not address
        // (we protected address, we store the function)
        m_originals[ index ] = vf_arr[ index ];

        // set hook
        vf_arr[ index ] = reinterpret_cast< uintptr_t >( override_fn );

        // restore protection
        succ = VirtualProtect(
            vf_arr + index,
            sizeof( void * ),
            old_prot,
            &old_prot
        );

        // expect to succeed restoting prot
        assert( succ );

        LOG_LOADING_END( "\"{}\" hooked {}", m_name.c_str( ), index );

        // yay we hooked successfully
        return true;
    }


    bool unhook( size_t index ) {
        LOG_LOADING( "\tunhook: step 0" );

        // dont unhook when no orig
        if ( !m_originals.contains( index ) )
            return false;

        LOG_LOADING( "\tunhook: step 1" );

        uintptr_t* vf_arr = reinterpret_cast< uintptr_t * >( m_vtbl_addr );

        DWORD old_prot;
        bool succ = VirtualProtect(
            reinterpret_cast< void * >( m_vtbl_addr + index * sizeof( void * ) ),
            sizeof( void * ),
            PAGE_EXECUTE_READWRITE,
            &old_prot
        );

        if ( !succ ) {
            printf( "Failed to protect memory: %d\n", GetLastError( ) );
            return false;
        }

        LOG_LOADING( "\tunhook: step 2" );

        // restore original
        vf_arr[ index ] = m_originals[ index ];

        // restore protection
        succ = VirtualProtect(
            vf_arr + index,
            sizeof( void * ),
            old_prot,
            &old_prot
        );

        // expect to succeed restoting prot
        assert( succ );

        m_originals.erase( index );

        LOG_LOADING_END( "\tunhook: step 3" );

        // yay we hooked successfully
        return true;
    }

    template< typename Fn >
    Fn get_original( size_t index ) {
        // mm how much does this affect performance?
        // iirc reinterpret was very heavy compared to
        // other casting ways, so might be better
        // doing it some other way (could store as uintptr_t ig?)
        return reinterpret_cast< Fn >( m_originals.at( index ) );
    }

    uintptr_t get_original_raw( const size_t index ) const {
        return m_originals.at( index );
    }

    void unhook_all( ) {
        // omg when removing while iterating map
        // it makes the iterator invalid
        // fix: store keys in a separate vector
        std::vector< size_t > keys( m_originals.size( ) );
        std::transform( m_originals.begin( ), m_originals.end( ), keys.begin( ),
                        [ ]( const auto& pair ) {
                            return pair.first;
                        } );

        for ( const auto idx : keys ) {
            LOG_INFO( "unhooking \"{}\" idx {}", m_name, idx );
            assert( unhook( idx ) );
            LOG_INFO( " -> success" );
        }
    }
};


#endif //VMT_HOOK_HH
