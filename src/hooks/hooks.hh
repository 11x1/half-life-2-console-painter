#ifndef HOOKS_HH
#define HOOKS_HH
#include <memory>
#include "types/vmt_hook.hh"

/*
hook def structure
namespace name {
    using def = return_type( call_type * )( args );
    inline def original { nullptr };
    inline size_t index = 0x8C / 4;
    inline void __stdcall hook( args again ) {
        ... our code
    };
}


all vftbl hooks by classname follow this init format

const auto vft = utils::get_vftable( "vgui2.dll", "VPanelWrapper" );
auto vmt = hooks::make_vmt_hook( vgui_panel_wrapper_vftable );

const bool succ = vmt.hook< ns::def >( ns::index, ns::hook );

if ( succ )
    ns::original = vmt.get_original< ns::def >( ns::index );
*/

#define MAKE_VFTABLE_HOOK( hook_name, idx, return_type, call_type, ... ) \
namespace all_hooks { \
    namespace hook_name { \
        using def = return_type ( call_type * )( __VA_ARGS__ ); \
        inline def original { nullptr };\
        inline size_t index = idx;\
        return_type call_type hook( __VA_ARGS__ );\
    } \
} \
return_type call_type all_hooks::hook_name::hook( __VA_ARGS__ )

#define INITIALIZE_VFTABLE_HOOK( module, classname, namespace_ ) { \
    { \
        const auto vft = utils::get_vftable( #module, #classname ); \
        auto vmt = hooks::make_vmt_hook( vft, #classname ); \
        const bool succ = vmt->hook< all_hooks::namespace_::def >( all_hooks::namespace_::index, all_hooks::namespace_::hook ); \
        if ( succ ) \
            all_hooks::namespace_::original = vmt->get_original< all_hooks::namespace_::def >( all_hooks::namespace_::index ); \
    } \
}

namespace hooks {
    inline std::vector< std::unique_ptr< vmt_hook > > m_hooks { };

    inline vmt_hook* make_vmt_hook( const uintptr_t vtbl_addr, const std::string& name = "<empty hook name>" ) {;
        m_hooks.emplace_back( std::make_unique< vmt_hook >( vtbl_addr, name ) );
        return m_hooks.at( m_hooks.size( ) - 1 ).get( );
    }

    inline void unhook_all( ) {
        for ( const auto& hook : m_hooks ) {
            hook->unhook_all( );
        }
    }
}



#endif //HOOKS_HH
