//
// Created by riis on 4/16/2025.
//

#include "module.hh"

uintptr_t module::scan_pattern( const std::string& pattern, const std::string& mask, size_t offset ) const {
    // nvm cant do this, pattern can have 0 bytes == null-terminator
    // assert( pattern.size( ) == mask.size( ) );

    return this->scan_pattern(
        reinterpret_cast< const byte* >( pattern.data( ) ),
        reinterpret_cast< const byte* >( mask.data( ) ),
        pattern.size( ),
        offset
    );
}

uintptr_t module::scan_pattern( const byte* pattern, const byte* mask, size_t pattern_size, size_t offset ) const {
    bool debug { false };
    byte* module_bytes = static_cast< byte * >( m_info.lpBaseOfDll );
    for ( size_t i = offset; i < m_info.SizeOfImage - pattern_size; i++ ) {
        bool failed { false };

        // if ( i == 0x1C86A0 ) {
        //     debug = true;
        // }

        // check through pattern bytes
        // use mask for pattern size, more reliable (same reason why no assert)
        for ( size_t pat_i = 0; pat_i < pattern_size; pat_i++ ) {
            // ? marks a wildcard
            if ( mask[ pat_i ] == '?' ) {
                if ( debug )
                    printf( "skipping pattern %d (?)\n", pat_i );
                continue;
            };
            const byte pattern_byte = pattern[ pat_i ];
            const byte module_byte = module_bytes[ i + pat_i ];
            if ( pattern_byte != module_byte ) {
                if ( debug ) printf( "%d @ pat_i ~= i + pat_i (%d %d)\n", pat_i, pattern_byte, module_byte );

                failed = true;
                break;
            }

            if ( debug )
                printf( "%d == %d (%d)", pat_i, i + pat_i, pattern[ pat_i ] );
        }

        if ( !failed ) {
            printf( "found pattern @ off=%#x\n", i );
            // our found match addr is at module bytes + i
            return reinterpret_cast< uintptr_t >( module_bytes + i );
        } else debug = false;
    }

    return 0;
}

size_t module::get_offset( const uintptr_t from_addr ) {
    return from_addr - reinterpret_cast< uintptr_t >( m_info.lpBaseOfDll );
}

