#ifndef MODELRENDER_HH
#define MODELRENDER_HH

#include "math.hh"
#include "material.hh"

struct model_t
{
    int fnHandle;
    char* strName;
    int pad8[3];
    // +4+16 (dec)
    int nLoadFlags;
    int nServerCount;
    material **ppMaterials;
    int type;
    int flags;
    Vector mins;
    Vector maxs;
    float radius;
    union
    {
        int brush;
        __int16 studio;
        int sprite;
    };
};


struct ModelRenderInfo_t
{
    Vector origin;
    QAngle angle;
    void *pRenderable;
    model_t *pModel;
    void *matrix3x4_model2world;
    void *matrix3x4_lightningoff;
    Vector *pLightningOrigin;
    int flags;
    int entity_index;
    int skin;
    int body;
    int hitboxset;
    __int16 instance;
};

struct DrawModelState_t
{
    void *m_pStudioHdr;
    void *m_pStudioHWData;
    void *m_pRenderable;
    void *m_pModelToWorld;
    int m_decals;
    int m_drawFlags;
    int m_lod;
};

namespace modelrender_internals {
    inline void (__stdcall *forced_material_override)(material *new_mat, int);
}

/*
class model_render;

struct model_render_vtbl
{
    void (__stdcall *forced_material_override)(material *new_mat, int);
    void *gap0[15];
    int (__thiscall *draw_model_ex)(model_render *, void *);
    void *gap44;
    bool (__thiscall *draw_model_setup)(model_render *, ModelRenderInfo_t *, DrawModelState_t *state, void *mat3x4_custombone2world, void **mat3x4_bone2worldout);
    void (__thiscall *draw_model_execute)(model_render *, DrawModelState_t *state, ModelRenderInfo_t *, void *mat3x4_bone2world);
};

class model_render {
private:
    model_render_vtbl* vtbl;

public:
    void forced_material_override( material* mat, int type = 0 ) {
        vtbl->forced_material_override( mat, type );
    }
};
*/

#endif //MODELRENDER_HH
