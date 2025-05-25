#ifndef COLOR_HH
#define COLOR_HH

class Color {
public:
    // constructors
    Color( ) {
        *( ( int * ) this ) = 0;
    }

    Color( int _r, int _g, int _b ) {
        SetColor( _r, _g, _b, 0 );
    }

    Color( int _r, int _g, int _b, int _a ) {
        SetColor( _r, _g, _b, _a );
    }

    explicit Color( const size_t hex ) {
        if ( hex <= 0xFF ) {
            _color[ 0 ] = _color[ 1 ] = _color[ 2 ] = _color[ 3 ] = static_cast< unsigned char >( hex );
        } else {
            // 0xRrGgBbAa
            _color[ 3 ] = ( hex & 0x000000FF ); // a
            _color[ 2 ] = ( hex & 0x0000FF00 ) >> 8; // b
            _color[ 1 ] = ( hex & 0x00FF0000 ) >> 16; // g
            _color[ 0 ] = ( hex & 0xFF000000 ) >> 24; // r
        }
    }

    // set the color
    // r - red component (0-255)
    // g - green component (0-255)
    // b - blue component (0-255)
    // a - alpha component, controls transparency (0 - transparent, 255 - opaque);
    void SetColor( int _r, int _g, int _b, int _a = 0 ) {
        _color[ 0 ] = ( unsigned char ) _r;
        _color[ 1 ] = ( unsigned char ) _g;
        _color[ 2 ] = ( unsigned char ) _b;
        _color[ 3 ] = ( unsigned char ) _a;
    }

    void GetColor( int& _r, int& _g, int& _b, int& _a ) const {
        _r = _color[ 0 ];
        _g = _color[ 1 ];
        _b = _color[ 2 ];
        _a = _color[ 3 ];
    }

    void SetRawColor( int color32 ) {
        _color[ 3 ] = ( color32 & 0x000000FF ); // a
        _color[ 2 ] = ( color32 & 0x0000FF00 ) >> 8; // b
        _color[ 1 ] = ( color32 & 0x00FF0000 ) >> 16; // g
        _color[ 0 ] = ( color32 & 0xFF000000 ) >> 24; // r
    }

    int GetRawColor( ) const {
        return *( ( int * ) this );
    }

    inline int r( ) const { return _color[ 0 ]; }
    inline int g( ) const { return _color[ 1 ]; }
    inline int b( ) const { return _color[ 2 ]; }
    inline int a( ) const { return _color[ 3 ]; }

    unsigned char& operator[]( int index ) {
        return _color[ index ];
    }

    const unsigned char& operator[]( int index ) const {
        return _color[ index ];
    }

    bool operator ==( const Color& rhs ) const {
        return ( *( ( int * ) this ) == *( ( int * ) &rhs ) );
    }

    bool operator !=( const Color& rhs ) const {
        return !( operator==( rhs ) );
    }

    Color& operator=( const Color& rhs ) {
        SetRawColor( rhs.GetRawColor( ) );
        return *this;
    }

private:
    unsigned char _color[ 4 ];
};

#endif //COLOR_HH
