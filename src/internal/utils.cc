#include "utils.hh"

#include <format>

uintptr_t utils::scan_pattern( const std::string& module_name, const std::string& pattern, const size_t offset ) {
    // expect to have the module defined already
    assert( m_modules.contains( module_name ) );

    const auto mod = m_modules.at( module_name ).get( );
    return mod->scan_pattern( pattern, offset );
}

uintptr_t utils::scan_pattern( const std::string& module_name, const std::vector< byte >& pattern, const std::vector< bool >& mask, size_t pattern_size, size_t offset ) {
    // expect to have the module defined already
    assert( m_modules.contains( module_name ) );

    const auto mod = m_modules.at( module_name ).get( );
    return mod->scan_pattern( pattern, mask, pattern_size, offset );
}

uintptr_t utils::get_vftable( const std::string& module_name, const std::string& class_name ) {
    const auto debug_mod = m_modules[ module_name ].get( );

    const std::string rtti_class_name = std::format( ".?AV{}@@", class_name );

    START_LIST( logger::prefixes::debug, std::format( "searching for \"{}\"'s vftable", class_name.c_str( ) ), 5, "        " );

    LOG_LIST( "rtti_class_name: {}", rtti_class_name.c_str( ) );

    const auto mask = std::string( rtti_class_name.size( ), 'x' );
    std::vector< byte > rtti_class_name_bytes { };
    for ( const auto& c : rtti_class_name )
        rtti_class_name_bytes.push_back( static_cast< byte >( c ) );

    const auto rtti_class_name_mask = std::vector< bool >( rtti_class_name_bytes.size( ), false );

    // could only search in .data section, maybe some other day
    const auto str_addr = utils::scan_pattern( module_name, rtti_class_name_bytes, rtti_class_name_mask, rtti_class_name_bytes.size( ) );

    if ( !str_addr ) return 0;

    LOG_LIST( "str_addr: 0x{:X}", debug_mod->get_offset( str_addr ) );

    // type info (32bit) is type descriptor name address
    // - 8 bytes
    // open up a type descriptor name in ida
    // the layout is something similiar
    //
    // Aa Bb Cc Dd       - type info address
    // 00 00 00 00       - rtti pad or whatever
    // 2E 3F 41 56 ...   - type descriptor name

    // fun fact, the mangled name for type descriptor
    // addresses is ??_R0?AV{classname}@@@8

    // now this cool type info is addressed by col
    // and col is referenced by the vftable (basically class method table)
    // for more info: https://blog.rop.la/en/reversing/2022/12/13/identifying-vftables-through-ms-cpp-rtti.html

    // @ 0xAaBbCcDd
    const auto type_info_addr = str_addr - 0x8;

    LOG_LIST( "type_info_addr: 0x{:X}", debug_mod->get_offset( type_info_addr ) );

    // cool thing about col (aka Complete Object Locator) is that
    // it has a recognisable signature (also references type info inside of it as said before)
    // the col structure is something like this
    //
    // 00 00 00 00   - signature
    // ?? ?? ?? ??   - offset
    // ?? ?? ?? ??   - offset to base class, if inherited
    // Dd Cc Bb Aa   - ptr to type info (le)
    //
    // since there might be many matching patterns
    // for the wanted col, we want to prioritise
    // the lowest offset + inheritance combo
    //
    // here offset means offset of this vtable in complete class (from top)
    // so since inherited vtables come after our
    // wanted vftable, we're looking for offset 0
    // (vftables will always be at the "top" of the object)

    std::vector< bool > col_mask {
        false, false, false, false,
         true,  true,  true,  true,
         true,  true,  true,  true,
        false, false, false, false
    };

    std::vector< byte > col_pattern {
        0, 0, 0, 0,
        0, 0, 0, 0, // this
        0, 0, 0, 0, // and this r skipped
        static_cast<byte>( type_info_addr       & 0xFF ), // Dd (lsb)
        static_cast<byte>( type_info_addr >>  8 & 0xFF ), // Cc
        static_cast<byte>( type_info_addr >> 16 & 0xFF ), // Bb
        static_cast<byte>( type_info_addr >> 24 & 0xFF ), // Aa
    };

    LOG_LIST( "col_pattern: 12*(0 0 0 0) + 0x{:X} 0x{:X} 0x{:X} 0x{:X}", col_pattern[ 12 ], col_pattern[ 13 ], col_pattern[ 14 ], col_pattern[ 15 ] );

    size_t off { 0 };
    uintptr_t last_vftbl { 0 };
    size_t last_biggest_offset_from_top { INT_MAX };
    while ( const auto col_pattern_match = utils::scan_pattern( module_name, col_pattern, col_mask, 16, off ) ) {
        // next scan starts at the end of out vftable
        off = debug_mod->get_offset( col_pattern_match ) + 16;

        LOG_LIST( "found col pattern match at 0x{:X}", debug_mod->get_offset( col_pattern_match ) );

        // find a ptr to col addr
        std::vector< byte > col_ptr_pat{
            static_cast<byte>( col_pattern_match       & 0xFF ), // Dd (lsb)
            static_cast<byte>( col_pattern_match >>  8 & 0xFF ), // Cc
            static_cast<byte>( col_pattern_match >> 16 & 0xFF ), // Bb
            static_cast<byte>( col_pattern_match >> 24 & 0xFF ), // Aa
        };

        // get offset from col, COL+0x04
        // col+0x04 = addr
        // deref addr for value
        const auto offset_from_top = *reinterpret_cast< int* >( col_pattern_match + 4 );

        const auto not_col = utils::scan_pattern( module_name, col_ptr_pat, { false, false, false, false }, 4 );

        LOG_LIST( "col ref @ 0x{:X} (off=0x{:X}) -> vftbl: 0x{:X} (off=0x{:X})", not_col, debug_mod->get_offset( not_col ), not_col + 0x4, debug_mod->get_offset( not_col + 0x4 ) );

        if ( offset_from_top < last_biggest_offset_from_top ) {
            last_vftbl = not_col + 0x4;
            last_biggest_offset_from_top = offset_from_top;

            // if it is our vftable, we can stop searching
            if ( offset_from_top == 0 )
                break;
        }
    };

    END_LIST( logger::prefixes::success, "found vftable at 0x{:X} with offfromtop={}", debug_mod->get_offset( last_vftbl ), last_biggest_offset_from_top );

    return last_vftbl;
}