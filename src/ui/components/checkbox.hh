#ifndef CHECKBOX_HH
#define CHECKBOX_HH

#include "base_component.hh"
#include "../../animations/animator.hh"

class checkbox : public base_component {
private:
  bool m_checked;
  bool m_hovered;

  animator< int > m_alpha_animator;
public:
  explicit checkbox( const std::string& name ) : base_component( 20, "checkbox", name ), m_checked( false ),
                                                 m_hovered( false ) {
    m_alpha_animator = animator< int >( 200 );
    m_alpha_animator.rawset_end( 80 );
  };

  void draw( ) override;
  void handle( ) override;
  void draw_debug( ) override;
};



#endif //CHECKBOX_HH
