#ifndef CRENDER_HH
#define CRENDER_HH

#include "math.hh"
#include "../internal/utils.hh"

/*
struct crender_vtbl
{
  void (__thiscall *frame_begin)(crender *);
  void (__thiscall *frame_end)(crender *);
  void (__thiscall *view_setup_vis)(crender *, bool, int, float **);
  void (__thiscall *view_draw_fade)(crender *, byte *, void *);
  void (__stdcall *draw_scene_begin)();
  void *pad14[2];
  void (__thiscall *build_world_lists)(crender *, int, int, int, int, char, int);
  void (__stdcall *draw_world_lists)(int, int, float);
  float *(__thiscall *view_origin)(crender *);
  float *(__stdcall *view_angles)();
  void *(__thiscall *view_get_current)(crender *);
  void *(__thiscall *view_matrix)(crender *);
  void *(__thiscall *world_to_screen_matrix)(crender *);
  float (__thiscall *get_framerate)(crender *);
  float (__thiscall *get_znear)(crender *);
  float (__thiscall *get_zfar)(crender *);
  float (__thiscall *get_fov)(crender *);
  float (__thiscall *get_fov_y)(crender *);
  void *pad4C;
  bool (__thiscall *clip_transform_with_projection)(crender *, float *, float *, int);
  bool (__stdcall *clip_transform)(float *, int);
  bool (__thiscall *screen_transform)(crender *, float *, float *);
  void *pad5C[11];
};
*/

class crender {
public:
    crender( ) = default;
    ~crender( ) = default;

    bool screen_transform( Vector& point, Vector* screen ) {
        // ScreenTransform( const Vector& point, Vector* pScreen );
        const auto func = utils::get_vfunc< bool ( __thiscall* )( crender *, Vector*, Vector* ) >( this, 0x58 / 4 );
        return func( this, &point, screen );
    }
};

#endif //CRENDER_HH
