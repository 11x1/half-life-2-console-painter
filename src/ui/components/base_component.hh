#ifndef BASE_COMPONENT_HH
#define BASE_COMPONENT_HH
#include <string>
#include <utility>
#include "../../renderer/vec2.hh"

class base_component {
private:
    std::string m_obj_name;

protected:
    std::string m_name;

    // no need to see these in any context
    // outside of components
    virtual void handle( const Vec2&, const Vec2& ) = 0;

    virtual void draw_debug( ) = 0;

public:
    explicit
    base_component( std::string obj_name = "base_component",
                    std::string name = "" ) : m_obj_name( std::move( obj_name ) ), m_name( std::move( name ) ) {
    };

    virtual ~base_component( ) = default;

    virtual void draw( int x, int y ) = 0;

    virtual std::string& get_class_name( ) {
        return m_obj_name;
    };
};

#endif //BASE_COMPONENT_HH
