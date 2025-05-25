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
MAKE_VFTABLE_HOOK( cmodelrender_drawmodelsetup, 18,
                   bool, __fastcall, void* thisptr, void* ecx, ModelRenderInfo_t* info, DrawModelState_t* state,
                   void *mat3x4_custombone2world, void **mat3x4_bone2worldout ) {


    if ( mats::test_material && *modelrender_internals::forced_material_override ) {
        modelrender_internals::forced_material_override( mats::test_material, 0 );
    }

    return original( thisptr, ecx, info, state, mat3x4_custombone2world, mat3x4_bone2worldout );
}

#endif //CMODELRENDER_DRAWMODELSETUP_HH
