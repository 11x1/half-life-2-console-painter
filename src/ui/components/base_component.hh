#ifndef BASE_COMPONENT_HH
#define BASE_COMPONENT_HH
#include <string>

class base_component {
private:
  int m_height;
  std::string m_obj_name;

protected:
  std::string m_name;

public:
  explicit base_component( const int height, const std::string& obj_name = "base_component", const std::string& name = "" ) : m_height( height ), m_obj_name( obj_name ), m_name( name ) { };
  virtual ~base_component( ) = default;

  virtual void draw( ) = 0;
  virtual void handle( ) = 0;

  virtual int get_height( ) {
    return m_height;
  };

  virtual std::string& get_class_name( ) {
    return m_obj_name;
  };

  virtual void draw_debug( ) = 0;
};

#endif //BASE_COMPONENT_HH
