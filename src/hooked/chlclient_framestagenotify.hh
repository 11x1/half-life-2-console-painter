#ifndef CHLCLIENT_FRAMESTAGENOTIFY_HH
#define CHLCLIENT_FRAMESTAGENOTIFY_HH
#include <cstdio>

#include "../hooks/hooks.hh"
#include "../input/input.hh"
#include "../sdk/FrameStage.hh"

MAKE_VFTABLE_HOOK( chlclient_framestagenotify, 0x8c / 4,
    void, __stdcall, int stage ) {

    switch ( stage ) {
        case FRAME_RENDER_START:
            break;
        default:
            break;
    }

    original( stage );
}

#endif //CHLCLIENT_FRAMESTAGENOTIFY_HH
