#ifndef CHLCLIENT_FRAMESTAGENOTIFY_HH
#define CHLCLIENT_FRAMESTAGENOTIFY_HH
#include <cstdio>
#include "../sdk/FrameStage.hh"

using std::string_literals::operator ""s;

inline auto watermark_text = "franzhack.cc.pub.llc.pub.co.uk.ai.ee";
inline auto watermark = std::string( watermark_text, watermark_text + strlen( watermark_text ) );

namespace chlclient_hook {
    // voidptr because i cba to define the class
    // just for initial testing if the vmt hook works,
    // this is fine for
    using def = void( __stdcall* )( int );
    inline def original { nullptr };
    inline size_t index = 0x8C / 4;
    inline void __stdcall hook( int stage ) {
        // printf( "[hook] framestage: %d original: %p\n", stage, original );

        switch ( stage ) {
            case FRAME_RENDER_START:
                break;
            default:
                break;
        }


        original( stage );
    };
}

#endif //CHLCLIENT_FRAMESTAGENOTIFY_HH
