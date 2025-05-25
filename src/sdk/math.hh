#ifndef MATH_HH
#define MATH_HH

#include "../renderer/vec2.hh"
#include <array>

struct Vector {
    float x, y, z;

    Vector( ) : x( 0 ), y( 0 ), z( 0 ) { };

    explicit Vector( const std::array< int, 2 >& a ) : x( static_cast< float >( a.at( 0 ) ) ), y( static_cast< float >( a.at( 1 ) ) ), z( 0 ) { };
    explicit Vector( const float a ) : x( a ), y( a ), z( a ) { };
    explicit Vector( const float x, const float y ) : x( x ), y( y ), z( 0 ) { };
    explicit Vector( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { };


    // if it's not explicit we can't use Vec2
    // in the constructor
    Vector( const Vec2& vec ) {
        x = vec.x;
        y = vec.y;
        z = 0;
    }
};

struct QAngle {
    float x, y, z;

    QAngle( ) : x( 0 ), y( 0 ), z( 0 ) { };
    explicit QAngle( const float a ) : x( a ), y( a ), z( a ) { };
    explicit QAngle( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { };
};

#endif //MATH_HH
