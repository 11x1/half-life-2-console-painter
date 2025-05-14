#ifndef TEXTENTRY_HH
#define TEXTENTRY_HH

struct textentry;

struct textentry_vtbl
{
    void *(__thiscall *get_vpanel)(textentry *);
    void *pad4[208];
    void (__thiscall *set_text)(textentry *, const char *);
    void *pad[2];
    void (__thiscall *get_text)(textentry *, char *buf, int buflen);
};


struct textentry
{
    textentry_vtbl *vtbl;
};

#endif //TEXTENTRY_HH
