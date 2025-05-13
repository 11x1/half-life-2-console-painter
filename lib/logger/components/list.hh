#ifndef LIST_HH
#define LIST_HH
#include <vector>

#include "base.hh"
#include "entry.hh"


namespace log::components {
    class list_t final : public base {
    private:
        std::vector< entry_t > m_entries{ };
        size_t m_max_rows{ 0 };

        std::string m_prefix { };
        color m_default_color;

    public:
        explicit list_t( const size_t max_rows ) : m_max_rows( max_rows ) {
        }

        // list specific
        void line( entry_t entry );
        void line( const std::string& str );
        void color( color col );

        // baseclass overrides
        list_t* spew( ) override;

        void update( ) override;

        list_t* prefix(const std::string& prefix) override;
        list_t* prefix(const log::components::prefix& prefix) override;
    };
}


#endif //LIST_HH
