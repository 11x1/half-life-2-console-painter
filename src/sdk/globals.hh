#ifndef GLOBALS_HH
#define GLOBALS_HH

#include <array>
#include "string.h"

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

    explicit Vector( const std::array< int, 2 >& a ) : x( static_cast< float >( a.at( 0 ) ) ), y( static_cast< float >( a.at( 1 ) ) ), z( 0 ) { };
    explicit Vector( const float a ) : x( a ), y( a ), z( a ) { };
    explicit Vector( const float x, const float y ) : x( x ), y( y ), z( 0 ) { };
    explicit Vector( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { };
};

struct Color {
    unsigned char _color[ 4 ];

    explicit Color( const size_t hex ) {
        if ( hex <= 0xFF ) {
            _color[ 0 ] = _color[ 1 ] = _color[ 2 ] = _color[ 3 ] = static_cast< unsigned char >( hex );
        }

        // 0xRrGgBbAa
        _color[ 3 ] = ( hex & 0x000000FF )      ;   // a
        _color[ 2 ] = ( hex & 0x0000FF00 ) >>  8;   // b
        _color[ 1 ] = ( hex & 0x00FF0000 ) >> 16;   // g
        _color[ 0 ] = ( hex & 0xFF000000 ) >> 24;   // r
    }
    Color( ) : _color { 0, 0, 0, 0 } { }
    Color( const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a ) : _color { r, g, b, a } { }
};

struct QAngle {
    float x, y, z;

    QAngle( ) : x( 0 ), y( 0 ), z( 0 ) { };
    explicit QAngle( const float a ) : x( a ), y( a ), z( a ) { };
    explicit QAngle( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { };
};

template <class T>
__forceinline void V_swap( T& x, T& y )
{
    T temp = x;
    x = y;
    y = temp;
}

template <class T>
inline void Destruct( T* pMemory )
{
    pMemory->~T();
}

template <class T>
inline T* CopyConstruct( T* pMemory, T const& src )
{
    return ::new( pMemory ) T(src);
}

#define V_memmove(dest, src, count)		memmove((dest), (src), (count))
#define Q_memmove				V_memmove


#endif //GLOBALS_HH
