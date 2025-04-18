#ifndef INTERNAL_HH
#define INTERNAL_HH

#include <map>
#include "../module/module.hh"

inline namespace internal {
    inline std::map< std::string, std::unique_ptr< module > > m_modules;

    namespace setup {
        void main( HINSTANCE dll_instance );

        void modules( );
    }

    namespace utils {
        uintptr_t scan_pattern( const std::string& module_name, const std::string& pattern, const std::string& mask, size_t offset = 0 );
        uintptr_t scan_pattern( const std::string& module_name, const byte* pattern, const byte* mask, size_t pattern_size, size_t offset = 0 );

        uintptr_t get_vftable( const std::string& module_name, const std::string& class_name );
    }
};



#endif //INTERNAL_HH
