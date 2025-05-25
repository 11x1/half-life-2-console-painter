#ifndef CHECKBOX_HH
#define CHECKBOX_HH

#include "base_component.hh"
#include "../../animations/animator.hh"

namespace component {
  class checkbox final : public base_component {
  private:
    bool* m_checked;
    bool m_hovered;
    int m_texture_id;

    animator< int > m_alpha_animator;
  protected:
    void handle( const Vec2&, const Vec2& ) override;
    void draw_debug( ) override;

  public:
    explicit checkbox( const std::string& name, bool* checked, const int texture_id ) : base_component( "checkbox", name ), m_checked( checked ),
                                                   m_hovered( false ), m_texture_id( texture_id ) {
      m_alpha_animator = animator< int >( 200 );
      m_alpha_animator.rawset_end( 80 );
    };

    void draw( int x, int y ) override;
  };
}



#endif //CHECKBOX_HH
