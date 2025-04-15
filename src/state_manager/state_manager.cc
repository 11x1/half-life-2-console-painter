//
// Created by riis on 4/15/2025.
//

#include "state_manager.hh"

void state_manager::set_state( EApplicationState state ) {
    m_state = state;
}

EApplicationState state_manager::get_state( ) const {
    return m_state;
}

const std::string& state_manager::get_state_name( ) const {
    return state_names[ this->get_state( ) ];
}
