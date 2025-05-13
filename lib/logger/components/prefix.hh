#ifndef INFO_LINE_HH
#define INFO_LINE_HH

#include "../color.hh"

namespace log::components {
    class prefix {
    private:
        std::string m_prefix;
        color m_color;
    public:
        explicit prefix( std::string prefix, color color ) : m_prefix( std::move( prefix ) ), m_color( std::move( color ) ) {
        }

        std::string get( ) const;
    };
}



#endif //INFO_LINE_HH
