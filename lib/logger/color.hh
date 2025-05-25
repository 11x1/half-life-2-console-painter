#ifndef LOGGER_COLOR_HH
#define LOGGER_COLOR_HH

#include "Windows.h"

#include <format>
#include <string>

class color {
private:
    byte m_r, m_g, m_b;
    bool m_has_been_set;

public:
    color( ) : m_r( 0 ), m_g( 0 ), m_b( 0 ), m_has_been_set( false ) {
    }

    color( const color& other ) = default;

    explicit color( const size_t rgb ) : m_has_been_set( true ) {
        if ( rgb <= 0xFF ) {
            m_r = m_g = m_b = static_cast< byte >( rgb );
        } else {
            // 0xRrGgBb
            m_r = static_cast< byte >( ( rgb & 0xFF0000 ) >> 16 );
            m_g = static_cast< byte >( ( rgb & 0x00FF00 ) >> 8  );
            m_b = static_cast< byte >( ( rgb & 0x0000FF ) );
        }
    }

    explicit color( const byte r, const byte g, const byte b ) : m_r( r ), m_g( g ), m_b( b ), m_has_been_set( true ) {
    }

    [[nodiscard]] std::string get_ansi_fg( ) const {
        constexpr auto fmt = "\x1B[38;2;{};{};{}m";
        return std::format( fmt, std::to_string( m_r ), std::to_string( m_g ), std::to_string( m_b ) );
    }

    [[nodiscard]] std::string get_ansi_bg( ) const {
        constexpr auto fmt = "\x1B[48;2;{};{};{}m";
        return std::format( fmt, std::to_string( m_r ), std::to_string( m_g ), std::to_string( m_b ) );
    }

    bool is_set( ) const {
        return m_has_been_set;
    }
};

inline const auto COLOR_NONE { color( ) };

#endif //LOGGER_COLOR_HH
