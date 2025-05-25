#include "game_funcs.hh"
#include "utils.hh"

void game_funcs::setup( ) {
    if ( const auto get_engine_version = utils::scan_pattern(
        "engine.dll",
        "A1 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC 55 8B EC 8B 45 ? 33 D2" )
    ) {
        game_funcs::get_engine_version = reinterpret_cast< get_engine_version_t >( get_engine_version );
        LOG( success, "found engine version getter: {}", game_funcs::get_engine_version( ) );
    } else {
        LOG( error, "failed to find engine version getter" );
    }
}
