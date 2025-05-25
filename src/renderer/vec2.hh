#ifndef VEC2_HH
#define VEC2_HH

struct Vec2 {
    int x, y;

    Vec2( ) = default;

    explicit Vec2( const int x, const int y ) : x( x ), y( y ) {
    };
};

#endif //VEC2_HH
