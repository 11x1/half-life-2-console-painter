#ifndef RICHTEXT_HH
#define RICHTEXT_HH
#include "../utils/utlvector.hh"

struct richtextinterior;
typedef unsigned char byte;
typedef unsigned int HFont;

struct /*VFT*/ richtext_vtbl
{
};

struct TFade
{
    float flFadeStartTime;
    float flFadeLength;
    float flFadeSustain;
    int  iOriginalAlpha;
};

// format stream - describes changes in formatting for the text stream
struct TFormatStream
{
    // render state
    Color color;
    int pixelsIndent;
    bool textClickable;
    unsigned short m_sClickableTextAction;

    TFade fade;

    // position in TextStream that these changes take effect
    int textStreamIndex;
};

struct richtext
{
    richtext_vtbl *vtbl;
    byte pad4[400];
    CUtlVector< wchar_t > m_text_buffer;
    CUtlVector< int > m_line_breaks;
    CUtlVector< TFormatStream >  *m_format_vector;
    byte pad1C0[12];
    int _cursorPos;
    bool _mouseSelection;
    bool _mouseDragSelection;
    int _select[2];
    int _pixelsIndent;
    int _maxCharCount;
    HFont _font;
    HFont m_hFontUnderline;
    int _selectionColor;
    int _selectionTextColor;
    bool _currentTextClickable;
    byte pad1F4[36];
    richtextinterior *m_pInterior;
};


#endif //RICHTEXT_HH
