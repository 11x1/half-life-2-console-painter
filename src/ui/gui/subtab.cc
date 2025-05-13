#include "subtab.hh"
#include "../components/checkbox.hh"

base_component* gui::subtab::get_component( const size_t idx ) const {
    return m_components.at( idx ).get( );
}

const std::vector< std::unique_ptr< base_component > >& gui::subtab::get_components( ) const {
    return m_components;
}

std::wstring gui::subtab::get_wstring_name( ) {
    static auto ws = std::wstring( m_name.begin( ), m_name.end( ) );
    return ws;
}

void gui::subtab::checkbox( const std::string& name, bool* ptr_checked ) {
    m_components.push_back( std::make_unique< component::checkbox >( name, ptr_checked ) );
}