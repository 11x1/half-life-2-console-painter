#ifndef MODELRENDER_HH
#define MODELRENDER_HH

#include "globals.hh"
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


struct __declspec(align(4)) ModelRenderInfo_t
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



#endif //MODELRENDER_HH
