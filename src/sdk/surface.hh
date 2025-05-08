#ifndef SURFACE_HH
#define SURFACE_HH
#include <string>

#include "globals.hh"
#include "../internal/utils.hh"

// fwddecl for vft struct
class surface;

typedef unsigned int HFont;
enum SurfaceFeature_e
{
  ANTIALIASED_FONTS = 0x1,
  DROPSHADOW_FONTS = 0x2,
  ESCAPE_KEY = 0x3,
  OPENING_NEW_HTML_WINDOWS = 0x4,
  FRAME_MINIMIZE_MAXIMIZE = 0x5,
  OUTLINE_FONTS = 0x6,
  DIRECT_HWND_RENDER = 0x7,
};

struct surface_vtbl
{
  void *pad0[10];
  void (__thiscall *set_draw_color_color)(surface *, Color);
  void (__thiscall *set_draw_color)(surface *, int, int, int, int);
  void (__thiscall *draw_filled_rect)(surface *, int, int, int, int);
  void (__thiscall *draw_filled_rect_array)(int **, int);
  void (__thiscall *draw_outlined_rect)(surface *, int, int, int, int);
  void (__thiscall *draw_line)(surface *, int, int, int, int);
  void (__thiscall *draw_poly_line)(surface *, int *px, int *py, int num);
  void (__thiscall *set_render_font)(surface *, unsigned int);
  void (__thiscall *set_text_color_color)(surface *, Color);
  void (__thiscall *set_text_color)(surface *, int, int, int, int);
  void (__thiscall *set_text_pos)(surface *, int, int);
  void (__thiscall *get_text_pos)(surface *, int *, int *);
  void (__thiscall *draw_text)(surface *, const wchar_t *, int, int);
  void (__thiscall *draw_unicode_char)(surface *, wchar_t, int);
  void (__thiscall *flush_text)(surface *);
  void *(__thiscall *create_html_window)(surface *, void *, void *);
  void *(__thiscall *paint_html_window)(surface *, void *);
  void *(__thiscall *delete_html_window)(surface *, void *);
  int (__thiscall *get_texture_id)(surface *, int);
  bool (__thiscall *get_texture_file)(surface *, int, char *, int);
  void (__thiscall *set_texture_file)(surface *, int, const char *, int, bool);
  void (__thiscall *set_texture_rgba)(surface *, int id, const unsigned __int8 *rgba, int w, int h, int filter, bool forcereload);
  void (__thiscall *set_texture)(surface *, int);
  void (__thiscall *get_texture_size)(surface *, int, int *, int *);
  void (__thiscall *draw_textured_rect)(surface *, int, int, int, int);
  void (__thiscall *is_texture_valid)(surface *, int);
  void (__thiscall *delete_texture)(surface *, int);
  int (__thiscall *new_texture_id)(surface *);
  void *pad98[14];
  void (__thiscall *set_cursor_always_visible)(surface *, bool);
  bool (__thiscall *is_cursor_visible)(surface *);
  void (__thiscall *apply_changes)(surface *);
  bool (__thiscall *is_within)(surface *, int x, int y);
  bool (__thiscall *has_focus)(surface *);
  bool (__thiscall *supports_feature)(surface *, SurfaceFeature_e);
  void *restrict_paint_to_single_panel;
  void *set_modal_panel;
  void *get_modal_panel;
  void *unlock_cursor;
  void *lock_cursor;
  void *set_translate_extended_keys;
  void *get_topmost_popup;
  void *set_top_level_focus;
  HFont (__thiscall *create_font)(surface *);
  void (__stdcall *set_font_glyph_set)(HFont, const char *, int h, int weight, int blur, int scanlines, int flags, int, int);
  bool (__thiscall *add_custom_font_file)(surface *, const char *, const char *);
  int (__thiscall *get_font_tall)(surface *, int);
  int (__thiscall *get_font_tall_requested)(surface *, int);
  int (__thiscall *get_font_ascent)(surface *, int);
  bool (__thiscall *is_font_additive)(surface *, int);
  void (__stdcall *get_char_abc_wide)(size_t, const wchar_t *, int *, int *, int *);
  void *pad128[48];
  bool (__thiscall *add_bitmap_font_file)(surface *, const char *);
  void (__thiscall *set_bitmap_font_name)(surface *, const char *, const char *);
  void *pad1F0[17];
  void (__thiscall *destroy_texture_id)(surface *, int);
};


class surface {
private:
  surface_vtbl* m_vftbl;
public:
  void draw_filled_rect( const Vector& pos2d, const Vector& sz2d ) {
    m_vftbl->set_draw_color( this, 255, 255, 255, 255 );
    m_vftbl->draw_filled_rect( this, pos2d.x, pos2d.y, pos2d.x + sz2d.x, pos2d.y + sz2d.y );
    // utils::get_vfunc< void (__thiscall *)(surface *, int, int, int, int) >( this, 0x2C / 4 )( this, 255, 255, 255, 255 );
    //utils::get_vfunc< void (__thiscall *)(surface *, int, int, int, int) >( this, 0x30 / 4 )( this, pos2d.x, pos2d.y, pos2d.x + sz2d.x, pos2d.y + sz2d.y );
  }

  void draw_text( std::wstring& text, int px, int py ) {
    // const auto set_text_pos = utils::get_vfunc< void (__thiscall *)(surface *, int, int) >( this, 0x50 / 4 );
    // const auto set_text_color = utils::get_vfunc< void (__thiscall *)( surface *, Color ) >( this, 0x48 / 4 );
    // const auto draw_text = utils::get_vfunc< void (__thiscall *)(surface *, const wchar_t *, int, int) >( this, 0x58 / 4 );

    m_vftbl->set_text_pos( this, px, py );
    m_vftbl->set_text_color_color( this, Color( 255, 255, 255, 120 ) );
    m_vftbl->draw_text( this, text.c_str( ), text.size( ), 0 );

    // set_text_pos( this, px, py );
    // set_text_color( this, Color( 255, 255, 255, 120 ) );
    // draw_text( this, text.c_str( ), text.size( ), 0 );
  }
};

#endif //SURFACE_HH
