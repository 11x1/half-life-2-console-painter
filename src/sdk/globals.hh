#ifndef GLOBALS_HH
#define GLOBALS_HH

struct globals_t {
    char gap0[12];
    float curtime;
    float frametime;
    int maxClients;
    int tickcount;
    float tickinterval;
    char gap1C[40];
    int eLoadType;
};

struct Vector {
    float x, y, z;

    Vector( ) : x( 0 ), y( 0 ), z( 0 ) { };
    explicit Vector( const float a ) : x( a ), y( a ), z( a ) { };
    explicit Vector( const float x, const float y ) : x( x ), y( y ), z( 0 ) { };
    explicit Vector( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { };
};

struct Color {
    unsigned char _color[ 4 ];

    Color( ) : _color { 0, 0, 0, 0 } { }
    Color( const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a ) : _color { r, g, b, a } { }
};

#endif //GLOBALS_HH
