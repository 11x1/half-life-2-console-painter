#ifndef HOOKER_HH
#define HOOKER_HH
#include <memory>
#include "types/vmt_hook.hh"

namespace hooks {
    inline std::unique_ptr< vmt_hook > make_vmt_hook( uintptr_t vtbl_addr ) {;
        return std::make_unique< vmt_hook >( vtbl_addr );
    }
}



#endif //HOOKER_HH
