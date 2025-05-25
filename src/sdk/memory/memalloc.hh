#ifndef MEMALLOC_HH
#define MEMALLOC_HH

class memalloc;

struct memalloc_vtbl {
    void* pad0;
    void*( __thiscall *alloc )( memalloc*, size_t );
    void* realloc2;
    void*( __thiscall *realloc )( memalloc*, void*, size_t );
    void* free2;
    void ( __thiscall *free )( memalloc*, void* );
    void* expand_nolongersupported;
    void* pad1C;
    void* get_size;
    void* pad24;
    void* pad28;
    void* pad2C;
    void* pad30;
    void* pad34;
    void* pad38;
    void* pad3C;
    void* pad40;
    void* pad44;
    void* dump_stats;
    void* pad4C;
    void* pad50;
    void* pad54;
    void* pad58;
    void* pad5C;
    void* pad60;
    void* pad64;
    void* pad68;
    void* pad6C;
    void* pad70;
    void* pad74;
    void* pad78;
    void* pad7C;
    void* pad80;
    void* pad84;
    void* pad88;
    void* pad8C;
    void* pad90;
    void* pad94;
    void* pad98;
    void* pad9C;
};

class memalloc {
private:
    memalloc_vtbl* vtbl;

public:
    void* alloc( size_t size ) {
        return vtbl->alloc( this, size );
    }

    void* realloc( void* ptr, size_t size ) {
        return vtbl->realloc( this, ptr, size );
    }

    void free( void* ptr ) {
        vtbl->free( this, ptr );
    }
};


#endif //MEMALLOC_HH
