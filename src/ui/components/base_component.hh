#ifndef BASE_COMPONENT_HH
#define BASE_COMPONENT_HH
#include <string>
#include "../../sdk/globals.hh"

class base_component {
private:
  int m_height;
  std::string m_obj_name;

protected:
  bool m_visible;
  std::string m_name;

  // no need to see these in any context
  // outside of components
  virtual void handle( const Vector&, const Vector& ) = 0;
  virtual void draw_debug( ) = 0;
public:
  explicit base_component( const int height, const std::string& obj_name = "base_component", const std::string& name = "" ) : m_height( height ), m_obj_name( obj_name ), m_name( name ), m_visible( true ) { };
  virtual ~base_component( ) = default;

  virtual void draw( ) = 0;

  virtual int get_height( ) {
    return m_height;
  };

  virtual std::string& get_class_name( ) {
    return m_obj_name;
  };

  virtual bool is_visible( ) {
    return m_visible;
  };

  virtual void set_visible( const bool state ) {
    m_visible = state;
  }

};

#endif //BASE_COMPONENT_HH
