#ifndef ENTRY_HH
#define ENTRY_HH

#include <string>

#include "prefix.hh"
#include "../color.hh"

enum e_entry_flags {
    BOLD = 1,
    UNDERLINE = 2,
};

namespace logger::components {
    class entry_t {
    private:
        std::string m_text;
        color m_fg;
        color m_bg;
        size_t m_flags{ 0 };

        std::string m_prefix{ };

    public:
        entry_t( ) = default;

        explicit entry_t( std::string text ) : m_text( std::move( text ) ), m_fg( 0xFF ) {
        }

        explicit entry_t( std::string text, const color& fg, const color& bg, const size_t flags = 0 ) : m_text( std::move( text ) ),
            m_fg( fg ),
            m_bg( bg ),
            m_flags( flags ) {
        }

        std::string get_content( ) const;
        size_t length( ) const;
        void spew( ) const;
        entry_t* prefix( const std::string& prefix );
        entry_t* prefix( const logger::components::prefix& prefix );

        void set_text( const std::string& text );
        void set_fg( const color& fg );
        void set_bg( const color& bg );
    };
}


#endif //ENTRY_HH
