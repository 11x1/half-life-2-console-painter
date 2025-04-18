#ifndef CHLCLIENT_FRAMESTAGENOTIFY_HH
#define CHLCLIENT_FRAMESTAGENOTIFY_HH
#include <cstdio>

namespace chlclient_hook {
    // voidptr because i cba to define the class
    // just for initial testing if the vmt hook works,
    // this is fine for
    using def = void( __stdcall* )( int );
    inline def original { nullptr };
    inline size_t index = 0x8C / 4;
    inline void __stdcall hook( int stage ) {
        printf( "[hook] framestage: %d original: %p\n", stage, original );

        original( stage );
    };
}

#endif //CHLCLIENT_FRAMESTAGENOTIFY_HH
