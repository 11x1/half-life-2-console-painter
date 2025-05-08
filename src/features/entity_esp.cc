#include "entity_esp.hh"

#include <cstdio>

#include "../internal/interfaces.hh"
#include "../sdk/globals.hh"
#include "../renderer/surface_wrapper.hh" // 🌯

namespace entity_esp {
    void run( ) {
        const auto highest_entity_index = interfaces::entitylist->get_highest_entity_index( );
        for ( int i = 0; i < highest_entity_index; ++i ) {
            const auto networked_ent = reinterpret_cast< uintptr_t* >( interfaces::entitylist->get_client_networkable( i ) );
            if ( !networked_ent ) continue;

            auto client_class = (*(int (__thiscall **)(uintptr_t *))(*networked_ent + 8))(networked_ent);
            const auto netname = *(const char **)(client_class + 8);

            /*
                -> get_abs_origin
                result = entitylist->vtbl->get_client_entity(entitylist, dword_1048D0CC + 1);
                if ( result )
                {
                  result = (uintptr_t *)(*(int (__thiscall **)(uintptr_t *))(*result + 36))(result);
                  this->m_vecLocation[0] = *(float *)result;
                  this->m_vecLocation[1] = *((float *)result + 1);
                  this->m_vecLocation[2] = *((float *)result + 2);
                }
             */

            const auto cliententity = reinterpret_cast< uintptr_t* >( interfaces::entitylist->get_client_entity( i ) );

            if ( !cliententity ) continue;

            const float* absorigin = (float *)(*(int (__thiscall **)(uintptr_t *))(*cliententity + 36))(cliententity);

            const auto pos = new Vector( absorigin[ 0 ], absorigin[ 1 ], absorigin[ 2 ] );

            Vector screen_pos { };

            bool is = interfaces::engine_renderer->screen_transform( *pos, &screen_pos );

            if ( screen_pos.x > 0 && screen_pos.x < 1200 && screen_pos.y > 0 && screen_pos.y < 675 ) {
                auto ws_netname = std::wstring( netname, netname + strlen( netname ) );

                screen_pos.y = 675 - screen_pos.y;
                surface_wrapper::draw_text( ws_netname, screen_pos, Color( 255, 255, 255, 255 ), fonts::entity_esp );
            }

            delete pos;
        }
    }
}
