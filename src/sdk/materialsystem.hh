#ifndef MATERIALSYSTEM_HH
#define MATERIALSYSTEM_HH

#include "material.hh"
#include "keyvalues/keyvalues.hh"

class materialsystem;

struct materialsystem_vtbl
{
    void *gap[70];
    material *(__thiscall *create_material)(materialsystem *, const char *, keyvalues *);
};

class materialsystem {
private:
    materialsystem_vtbl* vtbl;

public:
    material* create_material( const char* name, keyvalues* kv ) {
        return vtbl->create_material( this, name, kv );
    }
};

#endif //MATERIALSYSTEM_HH
