#ifndef VMT_HOOK_HH
#define VMT_HOOK_HH
#include <algorithm>

#include "Windows.h"

#include <cassert>
#include <cstdint>
#include <map>
#include <ranges>
#include <vector>


class vmt_hook {
private:
    uintptr_t m_vtbl_addr;

    // we set the original at index to map
    // when we hook sm
    std::map< size_t, uintptr_t > m_originals{ };

public:
    explicit vmt_hook( const uintptr_t vtbl_addr ) : m_vtbl_addr( vtbl_addr ) {
    }

    ~vmt_hook( ) { unhook_all( ); }

    template< typename Fn >
    bool hook( size_t index, Fn override_fn ) {
        printf( "hook: step 0\n" );

        // dont hook twice, otherwise we lose the original
        if ( m_originals.contains( index ) )
            return false;

        printf( "hook: step 1\n" );

        uintptr_t* vf_arr = reinterpret_cast< uintptr_t * >( m_vtbl_addr );

        DWORD old_prot;
        bool succ = VirtualProtect(
            // cant index, otherwise we're not
            // writing to the function address
            reinterpret_cast< void * >( m_vtbl_addr + index * sizeof( void * ) ),
            sizeof( void * ),
            PAGE_EXECUTE_READWRITE,
            &old_prot
        );

        printf( "tried to call VirtualProtect(%p, %d, %d, %d)\n",
                m_vtbl_addr + index * sizeof( void * ),
                sizeof( void * ),
                PAGE_EXECUTE_READWRITE,
                old_prot
        );

        if ( !succ ) {
            printf( "Failed to protect memory: %d\n", GetLastError( ) );
            return false;
        }

        printf( "hook: step 2\n" );

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

        // yay we hooked successfully
        return true;
    }


    bool unhook( size_t index ) {
        printf( "unhook: step 0\n" );

        // dont unhook when no orig
        if ( !m_originals.contains( index ) )
            return false;

        printf( "unhook: step 1\n" );

        uintptr_t* vf_arr = reinterpret_cast< uintptr_t * >( m_vtbl_addr );

        DWORD old_prot;
        bool succ = VirtualProtect(
            reinterpret_cast< void * >( m_vtbl_addr + index * sizeof( void * ) ),
            sizeof( void * ),
            PAGE_EXECUTE_READWRITE,
            &old_prot
        );

        printf( "unhook: tried to call VirtualProtect(%p, %d, %d, %d)\n",
                m_vtbl_addr + index * sizeof( void * ),
                sizeof( void * ),
                PAGE_EXECUTE_READWRITE,
                old_prot
        );

        if ( !succ ) {
            printf( "Failed to protect memory: %d\n", GetLastError( ) );
            return false;
        }

        printf( "unhook: step 2\n" );

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

        printf( "unhook: step 3\n" );

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

    void unhook_all( ) {
        // omg when removing while iterating map
        // it makes the iterator invalid
        // fix: store keys in a separate vector
        std::vector< size_t > keys ( m_originals.size( ) );
        std::transform( m_originals.begin( ), m_originals.end( ), keys.begin( ),
                        [ ]( const auto& pair ) {
                            return pair.first;
                        } );
        
        for ( const auto idx : keys ) {
            printf( " unhooking %d", idx );
            assert( unhook( idx ) );
            printf( " -> success\n" );
        }
    }
};


#endif //VMT_HOOK_HH
