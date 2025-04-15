#ifndef MAIN_HH
#define MAIN_HH
#include <Windows.h>

#include <cstdio>
#include <memory>
#include <vector>

// for ImDrawList type
#include "../../libs/imgui/imgui.h"

#include "../module/module.hh"

class function_thread {
private:
    HWND m_game_hwnd { nullptr };

    std::vector< std::unique_ptr< module > > m_modules { };
public:
    function_thread( ) = default;
    ~function_thread( ) {
        printf( "todo: unhooker here" );
    }

    void main( );

    bool setup( );
    void render( ImDrawList& draw_list );
};

inline function_thread g_function_thread;

#endif //MAIN_HH
