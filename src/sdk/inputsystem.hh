#ifndef INPUTSYSTEM_HH
#define INPUTSYSTEM_HH

#include "Windows.h"

class inputsystem;

struct inputevent_t
{
    int m_type;
    int m_tick;
    int m_data;
    int m_data2;
    int m_data3;
};

struct inputsystem_vtbl {
    void* pad0[ 5 ];
    void ( __thiscall *attach_to_window )( inputsystem*, HWND hwnd );
    void* pad18;
    void ( __thiscall *enable_input )( inputsystem*, bool );
    void ( __thiscall *enable_message_pump )( inputsystem*, bool );
    void* pad24;
    int ( __thiscall *get_poll_tick )( inputsystem* );
    void* pad2C[ 5 ];
    inputevent_t* ( __thiscall *get_event_count )( inputsystem* );
    void*( __thiscall *get_event_data )( inputsystem* );
    void* pad38[ 50 ];
};

class inputsystem {
private:
    inputsystem_vtbl* m_vftbl{ };
public:
};

#endif //INPUTSYSTEM_HH
