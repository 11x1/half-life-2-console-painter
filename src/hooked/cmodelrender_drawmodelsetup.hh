#ifndef CMODELRENDER_DRAWMODELSETUP_HH
#define CMODELRENDER_DRAWMODELSETUP_HH

#include <format>
#include <string>
#include "../sdk/modelrender.hh"

// bool __thiscall CModelRender::DrawModelSetup(
//      model_render *this,
//      ModelRenderInfo_t *info,
//      DrawModelState_t *state,
//      void *mat3x4_custombone2world,
//      void **mat3x4_bone2worldout
// )
// engine.dll+0x33BF64
// vft base @ 0x33BF1C
// idx = 0x33BF64 - 0x33BF1C = 72
// 72 / 4 = 18
namespace cmodelrender_drawmodelsetup {
    using def = bool( __fastcall* )( void* thisptr, void* ecx, ModelRenderInfo_t* info, DrawModelState_t* state, void *mat3x4_custombone2world, void **mat3x4_bone2worldout );
    inline def original { nullptr };
    inline size_t index = 18;
    inline bool __fastcall hook( void* thisptr, void* ecx, ModelRenderInfo_t* info, DrawModelState_t* state, void *mat3x4_custombone2world, void **mat3x4_bone2worldout ) {
        static constexpr const std::string_view debug_log = "name: {}\n";
        printf( std::format( debug_log, info->pModel->strName ).c_str( ) );
        return original( thisptr, ecx, info, state, mat3x4_custombone2world, mat3x4_bone2worldout );
    }
}

#endif //CMODELRENDER_DRAWMODELSETUP_HH
