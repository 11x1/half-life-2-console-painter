#ifndef CCONSOLEPANEL_HH
#define CCONSOLEPANEL_HH

struct cconsoledialog;
class keyvalues;
struct cconsolepanel;
struct richtext;
struct textentry;

struct cconsolepanel_vtbl
{
    void *pad0[35];
    void (__thiscall *post_message)(cconsolepanel *, keyvalues *);
    void *pad90[10];
    void (__thiscall *add_action_signal_target)(cconsolepanel *, cconsoledialog *);
    void *padBC;
    void (__thiscall *post_action_signal)(cconsolepanel *, keyvalues *);
};


struct cconsolepanel
{
    cconsolepanel_vtbl *vtbl;
    void *pad[111];
    richtext *m_pHistory;
    textentry *m_pEntry;
    void *gap1C8[5];
    char m_input_text[256];
    char m_last_input_text[256];
    bool m_bAutoCompleteMode;
    void *pad3DC[200];
};


#endif //CCONSOLEPANEL_HH
