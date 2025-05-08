//
// Created by riis on 4/16/2025.
//

#include "module.hh"

#include <format>
#include <sstream>

uintptr_t module::scan_pattern( const std::string& pattern, size_t offset ) const {
    // nvm cant do this, pattern can have 0 bytes == null-terminator
    // assert( pattern.size( ) == mask.size( ) );

    std::vector< byte > pattern_bytes { };
    std::vector< bool > mask { };
    size_t sz { 0 };

    // https://cplusplus.com/reference/sstream/istringstream/istringstream/
    std::istringstream pat_stream { pattern };
    std::string curbyte;

    while ( pat_stream >> curbyte ) {
        if ( curbyte == "?" ) {
            pattern_bytes.push_back( 0 );
            mask.push_back( true );
        } else if ( curbyte.size( ) == 2 && std::isxdigit( curbyte[ 0 ] ) && std::isxdigit( curbyte[ 1 ] ) ) {
            pattern_bytes.push_back( std::stoi( curbyte, nullptr, 16 ) );
            mask.push_back( false );
        } else {
            throw std::runtime_error {
                std::format( "encountered invalid character(s) \"{}\" in pattern \"{}\"", curbyte, pattern )
            };
        }
        sz++;
    }

    return this->scan_pattern(
        pattern_bytes,
        mask,
        sz,
        offset
    );
}

uintptr_t module::scan_pattern( const std::vector< byte >& pattern, const std::vector< bool >& mask, const size_t pattern_size, const size_t offset ) const {
    bool debug { false };
    byte* module_bytes = static_cast< byte * >( m_info.lpBaseOfDll );
    for ( size_t i = offset; i < m_info.SizeOfImage - pattern_size; i++ ) {
        bool failed { false };

        if ( i == 0x173695 ) {
            debug = true;
        }

        // check through pattern bytes
        for ( size_t pat_i = 0; pat_i < pattern_size; pat_i++ ) {
            // ? marks a wildcard
            if ( mask[ pat_i ] ) {
                if ( debug )
                    printf( "[wildcard] skipping pattern %d (?)\n", pat_i );
                continue;
            };
            const byte pattern_byte = pattern[ pat_i ];
            const byte module_byte = module_bytes[ i + pat_i ];
            if ( pattern_byte != module_byte ) {
                if ( debug ) printf( "[mismatch] pos=%d @ pat=%#X act=%#X\n", pat_i, pattern_byte, module_byte );

                failed = true;
                break;
            }

            if ( debug )
                printf( "[match] pos=%d %#X\n", pat_i, pattern_byte );
        }

        if ( !failed ) {
            printf( "[%s] found pattern @ off=%#X\n", get_filename( ).c_str( ), i );
            // our found match addr is at module bytes + i
            return reinterpret_cast< uintptr_t >( module_bytes + i );
        } else debug = false;
    }

    return 0;
}

HMODULE module::get_handle( ) const {
    return m_handle;
}

uintptr_t module::get_offset_address( const size_t off ) {
    return reinterpret_cast< uintptr_t >( m_info.lpBaseOfDll ) + off;
}

size_t module::get_offset( const uintptr_t from_addr ) {
    return from_addr - reinterpret_cast< uintptr_t >( m_info.lpBaseOfDll );
}

