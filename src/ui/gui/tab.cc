#include "tab.hh"
#include "subtab.hh"

gui::subtab* gui::tab::get_subtab( const size_t idx ) {
    return &m_subtabs[ idx ];
}

size_t gui::tab::get_selected_subtab( ) const {
    return m_selected_subtab;
}

void gui::tab::set_selected_subtab( const size_t sel ) {
    m_selected_subtab = sel;
}

std::wstring gui::tab::get_wstring_name( ) {
    static auto ws = std::wstring( m_name.begin( ), m_name.end( ) );
    return ws;
}

gui::subtab* gui::tab::subtab( const std::string& name ) {
    m_subtabs.push_back( gui::subtab( name ) );
    return &m_subtabs.at( m_subtabs.size( ) - 1 );
}