#ifndef GAME_FUNCS_HH
#define GAME_FUNCS_HH

class richtext;

namespace game_funcs {
    void setup( );

    using get_engine_version_t = int( __stdcall* )( );
    inline get_engine_version_t get_engine_version { nullptr };
};

#endif //GAME_FUNCS_HH
