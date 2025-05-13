#ifndef CHECKBOX_HH
#define CHECKBOX_HH

#include "base_component.hh"
#include "../../animations/animator.hh"

namespace component {
  class checkbox final : public base_component {
  private:
    bool* m_checked;
    bool m_hovered;

    animator< int > m_alpha_animator;
  protected:
    void handle( const Vector&, const Vector& ) override;
    void draw_debug( ) override;

  public:
    explicit checkbox( const std::string& name, bool* checked ) : base_component( 20, "checkbox", name ), m_checked( checked ),
                                                   m_hovered( false ) {
      m_alpha_animator = animator< int >( 200 );
      m_alpha_animator.rawset_end( 80 );
    };

    void draw( ) override;
  };
}



#endif //CHECKBOX_HH
