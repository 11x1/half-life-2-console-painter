#ifndef VPANEL_PAINT_TRAVERSE_HH
#define VPANEL_PAINT_TRAVERSE_HH

#include "../hooks/hooks.hh"
#include "../debug/debug.hh"
#include "../internal/globals.hh"
#include "../features/entity_esp.hh"
#include "../features/watermark.hh"
#include "../sdk/vgui/cconsolepanel.hh"
#include "../sdk/vgui/richtext.hh"
#include "../ui/gui/gui.hh"

/*
void __fastcall VPanelWrapper::PaintTraverse(void *this, void *edx0, int a2, bool a3, bool a4)
{
  int v5; // eax

  // this is VPanelWrapper::Client -> vgui::IClientPanel
  v5 = (*(int (__fastcall **)(void *, void *, int))(*(_DWORD *)this + 244))(this, edx0, a2);
  (*(void (__thiscall **)(int, bool, bool))(*(_DWORD *)v5 + 12))(v5, a3, a4);
}
*/

MAKE_VFTABLE_HOOK( vpanel_paint_traverse, 41,
                   void, __fastcall, void* a1, void* edx, void* a3, bool a4, bool a5 ) {
    /* old lua code, although for csgo
            local VPanel = ffi.cast( "uintptr_t**", a3 )
            local name = get_vtbl_func( VPanel, 35, "const char*(__thiscall*)(void*)" )( VPanel )
        */

    const auto name = utils::get_vfunc< const char*( __thiscall* )( void* ) >( a3, 35 )( a3 );

    const auto wstrname = std::wstring( name, name + strlen( name ) );
    if ( !debug::drawpanel_calls.contains( wstrname ) ) {
        debug::drawpanel_calls[ wstrname ] = 1;
    } else {
        debug::drawpanel_calls[ wstrname ] += 1;
    }

    if ( strcmp( name, "CHudViewport" ) == 0 ) {
        const auto time = std::chrono::steady_clock::now( );
        const long long time_ms = time.time_since_epoch( ).count( ) / 1000000;
        globals::m_current_time = time_ms;

        entity_esp::run( );
        hl2_watermark::run( );

        // g_gui.draw( );

        debug::draw( );
    }

    if ( strcmp( name, "CConsolePanel" ) == 0 ) {
        // returns a IClientPanel*
        // but since CConsolePanel inherits
        // from IClientPanel, we can cast it
        const auto console_panel = utils::get_vfunc< cconsolepanel* (__fastcall *)(void *, void*, void*) >( a1, 244 / 4 )( a1, edx, a3 );

        static bool init { false };
        if ( !init ) {
            printf( "%ls\n", &console_panel->m_pHistory->m_text_buffer.Head( ) );
            init = true;

            const std::wstring a = L"- HELLO THIS IS A TEST -";
            for ( const auto& ch : a ) {
                console_panel->m_pHistory->m_text_buffer.InsertAfter( 100, ch );
            }
        }

        const auto textinput = console_panel->m_input_text;

        const auto wstr_input = std::wstring( textinput, textinput + 256 );

        surface_wrapper::draw_text( wstr_input, Vector( 400, 50 ), Color( 0xFF ), fonts::debug );
    }

    original( a1, edx, a3, a4, a5 );
};

#endif //VPANEL_PAINT_TRAVERSE_HH
