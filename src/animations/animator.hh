#ifndef ANIMATOR_HH
#define ANIMATOR_HH

#include <algorithm>
#include <format>
#include "../internal/globals.hh"
#include "../renderer/surface_wrapper.hh"
#include "../sdk/globals.hh"


template< typename T  > requires std::is_arithmetic_v< T >
class animator {
private:
    long long m_last_time{ };
    long long m_anim_time_ms{ };
    T m_start;
    T m_end;

public:
    animator( ) = default;

    explicit animator( const long long anim_time_ms ) : m_anim_time_ms( anim_time_ms ) {
    };

    void set_time( long long time_ms ) {
        m_anim_time_ms = time_ms;
    }

    T get_end( ) const {
        return m_end;
    }

    T get_start( ) const {
        return m_start;
    }

    double get_pc( ) {
        const double pc = std::clamp< double >(
            static_cast< double >( globals::m_current_time - m_last_time ) / m_anim_time_ms, 0, 1 );

        return pc;
    }

    T get_value( ) {
        return m_start + ( m_end - m_start ) * get_pc( );
    }

    void rawset_start( T new_start ) {
        m_start = new_start;
    }

    void rawset_end( T new_end ) {
        m_end = new_end;
    }

    void reset( ) {
        m_start = T( );
        m_end = T( );
    }

    void update_end( T new_end ) {
        const auto pc = get_pc( );

        m_start = m_start + ( m_end - m_start ) * pc;
        m_end = new_end;

        static constexpr auto log_fmt = "{} updated end to {} end={} (start={})\n";

        printf( std::format( log_fmt, __FUNCTION__, new_end, m_end, m_start ).c_str( ) );

        m_last_time = globals::m_current_time;
    }

    void draw_debug( const Vector& pos ) {
        constexpr auto debug_fmt = L"m_last_time={} m_anim_time_ms={} m_start={} m_end={} get_value()={} get_pc()={}";
        const auto ws_debug_str = std::format(
            debug_fmt,
            m_last_time,
            m_anim_time_ms,
            m_start,
            m_end,
            get_value( ),
            get_pc( )
        );

        surface_wrapper::draw_text( ws_debug_str, pos, Color( 255, 0, 0, 255 ), fonts::watermark_font );
    }
};


#endif //ANIMATOR_HH
