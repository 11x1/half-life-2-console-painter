#ifndef SUBTAB_HH
#define SUBTAB_HH
#include <memory>
#include <string>
#include <vector>
#include "../components/base_component.hh"


namespace gui {
    class subtab {
    private:
        std::string m_name;
        std::vector< std::unique_ptr< base_component > > m_components;

    public:
        explicit subtab( const std::string& name ) : m_name( name ) { }

        size_t get_element_count( ) const { return m_components.size( ); }
        base_component* get_component( const size_t idx ) const;

        const std::vector< std::unique_ptr< base_component > >& get_components( ) const;

        std::wstring get_wstring_name( );
        void checkbox( const std::string& name, bool* ptr_checked );
    };
}



#endif //SUBTAB_HH
