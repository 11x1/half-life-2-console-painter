#ifndef SURFACE_HH
#define SURFACE_HH
#include <string>

#include "math.hh"
#include "common/color.hh"
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

enum EFontFlags
{
  FONTFLAG_NONE,
  FONTFLAG_ITALIC			= 0x001,
  FONTFLAG_UNDERLINE		= 0x002,
  FONTFLAG_STRIKEOUT		= 0x004,
  FONTFLAG_SYMBOL			= 0x008,
  FONTFLAG_ANTIALIAS		= 0x010,
  FONTFLAG_GAUSSIANBLUR	= 0x020,
  FONTFLAG_ROTARY			= 0x040,
  FONTFLAG_DROPSHADOW		= 0x080,
  FONTFLAG_ADDITIVE		= 0x100,
  FONTFLAG_OUTLINE		= 0x200,
  FONTFLAG_CUSTOM			= 0x400,
  FONTFLAG_BITMAP			= 0x800,
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
  int (__thiscall *get_texture_id)(surface *, const char *);
  bool (__thiscall *get_texture_file)(surface *, int, char *, int);
  void (__thiscall *set_texture_file)(surface *, int, const char *, int, bool);
  void (__thiscall *set_texture_rgba)(surface *, int id, const unsigned __int8 *rgba, int w, int h, int filter, bool forcereload);
  void (__thiscall *set_texture)(surface *, int);
  void (__thiscall *get_texture_size)(surface *, int, int *, int *);
  void (__thiscall *draw_textured_rect)(surface *, int, int, int, int);
  bool (__thiscall *is_texture_valid)(surface *, int);
  bool (__thiscall *delete_texture)(surface *, int);
  int (__thiscall *new_texture_id)(surface *);
  void (__thiscall *get_screen_size)(surface *, int *, int *);
  void *pad98[13];
  void (__thiscall *set_cursor_always_visible)(surface *, bool);
  bool (__thiscall *is_cursor_visible)(surface *);
  void (__thiscall *apply_changes)(surface *);
  bool (__thiscall *is_within)(surface *, int x, int y);
  bool (__thiscall *has_focus)(surface *);
  bool (__thiscall *supports_feature)(surface *, SurfaceFeature_e);
  void *restrict_paint_to_single_panel; // relying on the hl2 src for these ones
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
  void (__stdcall *get_char_abc_wide)(size_t, const int, int *, int *, int *);
  int (__thiscall *get_character_width)(surface *, HFont, int ch);
  void (__thiscall *get_text_size)(surface *, HFont, const wchar_t *, int *, int *);
  void *pad130[46];
  bool (__thiscall *add_bitmap_font_file)(surface *, const char *);
  void (__thiscall *set_bitmap_font_name)(surface *, const char *, const char *);
  void *pad1F0[17];
  void (__thiscall *destroy_texture_id)(surface *, int);
  void *pad238[20];
  void (__stdcall *draw_colored_text)(surface *, HFont, int, int, int r, int g, int b, int a, const char *fmt);
  void *pad28C[2];
  int (*draw_text_len)(surface *, HFont, const char *fmt, ...);
};


class surface {
private:
  surface_vtbl* m_vftbl;
public:
  void set_draw_color( const Color col ) {
    m_vftbl->set_draw_color_color( this, col );
  }

  void set_draw_color( const int r, const int g, const int b, const int a ) {
    m_vftbl->set_draw_color( this, r, g, b, a );
  }

  void draw_filled_rect( const Vector& pos2d, const Vector& sz2d ) {
    m_vftbl->draw_filled_rect( this, pos2d.x, pos2d.y, pos2d.x + sz2d.x, pos2d.y + sz2d.y );
  }

  void draw_outlined_rect( const Vector& pos2d, const Vector& sz2d ) {
    m_vftbl->draw_outlined_rect( this, pos2d.x, pos2d.y, pos2d.x + sz2d.x, pos2d.y + sz2d.y );
  }

  void draw_line( const Vector& start, const Vector& end ) {
    m_vftbl->draw_line( this, start.x, start.y, end.x, end.y );
  }

  void draw_poly_line( const std::vector< Vector >& points ) {
    std::vector< int > x, y;
    for ( const auto& point : points ) {
      x.push_back( static_cast< int >( point.x ) );
      y.push_back( static_cast< int >( point.y ) );
    }
    m_vftbl->draw_poly_line( this, x.data( ), y.data( ), points.size( ) );
  }

  void set_render_font( const HFont font ) {
    m_vftbl->set_render_font( this, font );
  }

  void set_text_color( const Color col ) {
    m_vftbl->set_text_color_color( this, col );
  }

  void set_text_color( const int r, const int g, const int b, const int a ) {
    m_vftbl->set_text_color( this, r, g, b, a );
  }

  void set_text_pos( const Vector& pos2d ) {
    m_vftbl->set_text_pos( this, pos2d.x, pos2d.y );
  }

  Vector get_text_pos( ) {
    static int x, y;
    m_vftbl->get_text_pos( this, &x, &y );
    return Vector( x, y );
  }

  void draw_text( const std::wstring& text ) {
    m_vftbl->draw_text( this, text.c_str( ), text.size( ), 0 );
  }

  void draw_unicode_char( const wchar_t c ) {
    m_vftbl->draw_unicode_char( this, c, 0 );
  }

  void flush_text( ) {
    m_vftbl->flush_text( this );
  }

  // no html yet

  int get_texture_id( const std::string& texture_name ) {
    return m_vftbl->get_texture_id( this, texture_name.c_str( ) );
  }

  void set_texture_rgba( const int id, const std::vector< byte >& rgba, const int w, const int h, const int filter = 0, const bool forcereload = false ) {
    m_vftbl->set_texture_rgba( this, id, rgba.data( ), w, h, filter, forcereload );
  }

  void set_draw_texture( const int id ) {
    m_vftbl->set_texture( this, id );
  }

  Vector get_texture_size( const int id ) {
    static int w, h;
    m_vftbl->get_texture_size( this, id, &w, &h );
    return Vector( w, h );
  }

  void draw_textured_rect( const Vector& pos2d, const Vector& sz2d ) {
    m_vftbl->draw_textured_rect( this, pos2d.x, pos2d.y, pos2d.x + sz2d.x, pos2d.y + sz2d.y );
  }

  bool is_texture_valid( const int id ) {
    return m_vftbl->is_texture_valid( this, id );
  }

  bool delete_texture( const int id ) {
    return m_vftbl->delete_texture( this, id );
  }

  int create_texture( ) {
    return m_vftbl->new_texture_id( this );
  }

  HFont create_font( ) {
    return m_vftbl->create_font( this );
  }

  void set_font_glyph_set( const HFont font, const std::string& name, const int tall, const int weight, const int blur, const int scanlines, const int flags, const int nmin = 0, const int nmax = 0 ) {
    m_vftbl->set_font_glyph_set( font, name.c_str( ), tall, weight, blur, scanlines, flags, nmin, nmax );
  }

  Vector get_text_size( const HFont font, const std::wstring& text ) {
    static int w, h;
    m_vftbl->get_text_size( this, font, text.c_str( ), &w, &h );
    return Vector( w, h );
  }

  Vector get_screen_size( ) {
    static int w, h;
    m_vftbl->get_screen_size( this, &w, &h );
    return Vector( w, h );
  }

  int get_character_width( const HFont font, const wchar_t ch ) {
    return m_vftbl->get_character_width( this, font, ch );
  }

  void get_char_abc_wide( const size_t font, const int ch, int* a, int* b, int* c ) {
    m_vftbl->get_char_abc_wide( font, ch, a, b, c );
  }
};

#endif //SURFACE_HH
