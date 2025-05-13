#ifndef TAB_HH
#define TAB_HH
#include <string>
#include <vector>

#include "subtab.hh"

namespace gui {
    class tab {
    private:
        size_t m_selected_subtab { 0 };
        std::string m_name;
        std::vector< gui::subtab > m_subtabs;

    public:
        explicit tab( const std::string& name ) : m_name( name ) { }

        size_t get_subtab_count( ) const { return m_subtabs.size( ); }
        gui::subtab* get_subtab( const size_t idx );

        size_t get_selected_subtab( ) const;
        void set_selected_subtab( size_t );

        std::wstring get_wstring_name( );
        gui::subtab* subtab( const std::string& name );
    };
}

#endif //TAB_HH
