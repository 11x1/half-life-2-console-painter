#ifndef LINE_HH
#define LINE_HH
#include "base.hh"
#include "entry.hh"
#include "prefix.hh"

namespace log::components {
    class line final : public base {
    protected:
        entry_t m_entry;

        log::components::prefix* m_prefix { nullptr };
    public:
        explicit line( entry_t entry ) : m_entry( std::move( entry ) ) {
        }

        explicit line( const std::string& text ) : m_entry( text ) {
        }

        explicit line( const std::string& text, const color& fg, const color& bg, const size_t flags = 0 ) : m_entry( text, fg, bg, flags ) {
        }

        // line specific
        void update_entry( entry_t entry );
        void update_entry( const std::string& text );
        line* color( const color& fg, const color& bg = COLOR_NONE );

        // overloads
        line* spew( ) override;
        void update( ) override;
        line* prefix(const std::string& prefix) override;
        line* prefix(const log::components::prefix& prefix) override;
    };
}



#endif //LINE_HH
