#ifndef CCONSOLEDIALOG_HH
#define CCONSOLEDIALOG_HH

struct cconsolepanel;

struct /*VFT*/ cconsoledialog_vtbl
{
};


struct cconsoledialog
{
    cconsoledialog_vtbl *vtbl;
    void *pad4[147];
    cconsolepanel *m_pconsole_panel;
};

#endif //CCONSOLEDIALOG_HH
