#ifndef INTERFACES_HH
#define INTERFACES_HH
#include "../sdk/client_entitylist.hh"
#include "../sdk/crender.hh"
#include "../sdk/surface.hh"

namespace interfaces {
    void setup( );

    inline client_entitylist* entitylist { nullptr };
    inline crender* engine_renderer { nullptr };
    inline surface* c_surface { nullptr };
    inline uintptr_t* vgui_panel { nullptr };
};



#endif //INTERFACES_HH
