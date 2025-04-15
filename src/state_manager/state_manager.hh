//
// Created by riis on 4/15/2025.
//

#ifndef STATE_MANAGER_HH
#define STATE_MANAGER_HH
#include <map>
#include <string>

enum EApplicationState {
    FAILED,
    STARTING,
    WAITING_FOR_WINDOW,
    FOUND_WINDOW,
    FOUND_MODULES,
    RUNNING,
};

inline std::map< EApplicationState, std::string > state_names {
    { FAILED, "FAILED" },
    { STARTING, "STARTING" },
    { WAITING_FOR_WINDOW, "WAITING FOR WINDOW" },
    { FOUND_WINDOW, "FOUND WINDOW" },
    { FOUND_MODULES, "FOUND MODULES" },
    { RUNNING, "RUNNING" }
};

class state_manager {
private:
    EApplicationState m_state { STARTING };
public:
  void set_state( EApplicationState state );

    [[nodiscard]] EApplicationState get_state( ) const;

    [[nodiscard]] const std::string& get_state_name( ) const;
};

inline state_manager g_state_manager { };


#endif //STATE_MANAGER_HH
