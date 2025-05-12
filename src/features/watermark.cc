#include "watermark.hh"

#include <format>

using std::string_literals::operator ""s;

void hl2_watermark::run( ) {
    static constexpr auto watermark_fmt = L"hl2 external | username: {} | time: {}:{}";

    static SYSTEMTIME systime;
    GetSystemTime( &systime );

    const auto watermark_text = std::format( watermark_fmt, L"franz", systime.wHour, systime.wMinute );

    const Vector watermark_text_size = surface_wrapper::get_text_size( watermark_text, fonts::watermark_font );

    const auto watermark_pos = Vector(
        surface_wrapper::get_screen_size( ).x - margin.x - pad.x * 2 - watermark_text_size.x,
        margin.x
    );

    const auto watermark_size = Vector(
        watermark_text_size.x + pad.x * 2,
        watermark_text_size.y + pad.y * 2
    );

    const auto watermark_text_pos = Vector(
        watermark_pos.x + pad.x,
        watermark_pos.y + pad.y
    );

    static auto watermark_col = Color( 0, 0, 0, 255 );
    static auto watermark_outline_col = Color( 225, 102, 102, 255 );
    static auto watermark_text_col = Color( 255, 255, 255, 255 );

    surface_wrapper::draw_rectangle_filled( watermark_pos, watermark_size, watermark_col );
    surface_wrapper::draw_rectangle( watermark_pos, watermark_size, watermark_outline_col );
    surface_wrapper::draw_text( watermark_text, watermark_text_pos, watermark_text_col, fonts::watermark_font );

    m_watermark_checkbox.draw( );
}
