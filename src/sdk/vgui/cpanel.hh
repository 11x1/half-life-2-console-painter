#ifndef CPANEL_HH
#define CPANEL_HH

struct cpanel;

struct cpanel_vtbl
{
    void *pad0[3];
    void (__thiscall *get_pos)(cpanel*, void* VPANEL, int *x, int *y);
    void* pad10;
    void (__thiscall *get_size)(cpanel *, void *VPANEL, int *w, int *h);
};


class cpanel {
private:
    cpanel_vtbl *vtbl { };

public:
    Vector get_panel_pos( void *VPANEL ) {
        static int x, y;
        vtbl->get_pos( this, VPANEL, &x, &y );
        return Vector( x, y, 0 );
    }

    Vector get_panel_size( void *VPANEL ) {
        static int w, h;
        vtbl->get_size( this, VPANEL, &w, &h );
        return Vector( w, h, 0 );
    }
};


#endif //CPANEL_HH
