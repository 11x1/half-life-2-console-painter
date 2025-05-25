#ifndef INTERFACES_HH
#define INTERFACES_HH
#include "../sdk/client_entitylist.hh"
#include "../sdk/crender.hh"
#include "../sdk/surface.hh"
#include "../sdk/memory/memalloc.hh"
#include "../sdk/steamapi/steamutils.hh"
#include "../sdk/vgui/cpanel.hh"

namespace interfaces {
    void setup( );

    inline steamutils* steamutils { nullptr };
    inline client_entitylist* entitylist { nullptr };
    inline crender* engine_renderer { nullptr };
    inline surface* c_surface { nullptr };
    inline cpanel* vgui_panel { nullptr };
    inline ::memalloc* memalloc { nullptr };
};



#endif //INTERFACES_HH
