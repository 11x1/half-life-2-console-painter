#ifndef SLIDER_HH
#define SLIDER_HH
#include "base_component.hh"

#include "../../animations/animator.hh"

namespace component {
    class slider final : public base_component {
    private:
        int m_min;
        int m_max;
        int m_width { 100 }; // 300px by default
        int* m_ref_value;

        animator< int > m_value_animator;
    protected:
        void handle( const Vec2&, const Vec2& ) override;
        void draw_debug( ) override;

    public:
        explicit slider( const std::string& name, int* cfg_el, const int min, const int max )
            : base_component( "slider", name ),
              m_min( min ), m_max( max ),
              m_ref_value( cfg_el )
        {
            m_value_animator = animator< int >( 200 );
            m_value_animator.rawset_end( *cfg_el );
        };

        void draw( const int x, const int y ) override;
    };
}



#endif //SLIDER_HH
