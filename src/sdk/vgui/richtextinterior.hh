#ifndef RICHTEXTINTERIOR_HH
#define RICHTEXTINTERIOR_HH

typedef unsigned char byte;

struct /*VFT*/ richtextinterior_vtbl
{
};

struct richtextinterior
{
    richtextinterior_vtbl *vtbl;
    byte pad4[472];
};


#endif //RICHTEXTINTERIOR_HH
