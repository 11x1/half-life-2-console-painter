#ifndef CGAMECONSOLEDIALOG_HH
#define CGAMECONSOLEDIALOG_HH

struct cconsolepanel;

struct /*VFT*/ cgameconsoledialog_vtbl
{
};


struct cgameconsoledialog
{
    cgameconsoledialog_vtbl *vtbl;
    void *pad[147];
    cconsolepanel *m_pConsolePanel;
};

#endif //CGAMECONSOLEDIALOG_HH
