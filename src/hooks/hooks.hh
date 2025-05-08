#ifndef HOOKER_HH
#define HOOKER_HH
#include <memory>
#include "types/vmt_hook.hh"

namespace hooks {
    inline std::vector< vmt_hook > m_hooks { };

    inline vmt_hook make_vmt_hook( const uintptr_t vtbl_addr ) {;
        auto hook = vmt_hook( vtbl_addr );
        m_hooks.push_back( hook );
        return hook;
    }

    inline void unhook_all( ) {
        for ( auto& hook : m_hooks ) {
            hook.unhook_all( );
        }
    }
}



#endif //HOOKER_HH
