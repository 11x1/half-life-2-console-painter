#include "watermark.hh"

#include <format>

using std::string_literals::operator ""s;

void hl2_watermark::run( ) {
    static constexpr auto watermark_fmt = L"hl2 internal (oops) | {} | b: {} | {}:{}";

    static SYSTEMTIME systime;
    GetSystemTime( &systime );

    wchar_t sysname[ 32 ];
    static DWORD sysname_len = 32; // why does it NEED to be a dword
    GetUserNameW( sysname, &sysname_len );

    const auto name = std::wstring( sysname );

    static auto compile_time = ( [ ]( ) -> std::wstring {
        // "Mmm dd yyyy"
        const auto date = __DATE__;

        wchar_t month[ 4 ]( 0 );
        wchar_t day[ 3 ]( 0 );

        // read month to buf
        for ( int i = 0; i < 3; i++ )
            month[ i ] = static_cast< wchar_t >( date[ i ] );

        const wchar_t* month_num;

        if ( wcscmp( month, L"Jan" ) == 0 ) month_num = L"01";
        else if ( wcscmp( month, L"Feb" ) == 0 ) month_num = L"02";
        else if ( wcscmp( month, L"Mar" ) == 0 ) month_num = L"03";
        else if ( wcscmp( month, L"Apr" ) == 0 ) month_num = L"04";
        else if ( wcscmp( month, L"May" ) == 0 ) month_num = L"05";
        else if ( wcscmp( month, L"Jun" ) == 0 ) month_num = L"06";
        else if ( wcscmp( month, L"Jul" ) == 0 ) month_num = L"07";
        else if ( wcscmp( month, L"Aug" ) == 0 ) month_num = L"08";
        else if ( wcscmp( month, L"Sep" ) == 0 ) month_num = L"09";
        else if ( wcscmp( month, L"Oct" ) == 0 ) month_num = L"10";
        else if ( wcscmp( month, L"Nov" ) == 0 ) month_num = L"11";
        else if ( wcscmp( month, L"Dec" ) == 0 ) month_num = L"12";
        else month_num = L"??";

        for ( int i = 4; i < 6; i++ )
            day[ i - 4 ] = static_cast< wchar_t >( date[ i ] );

        if ( day[ 0 ] == ' ' )
            day[ 0 ] = '0';

        return std::format( L"{}/{}", day, month_num );
    } )( );

    const auto watermark_text = std::format( watermark_fmt, name, compile_time, systime.wHour, systime.wMinute );

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
}
