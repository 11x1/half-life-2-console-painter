#ifndef MEM_FUNCS_HH
#define MEM_FUNCS_HH

#include "../../internal/interfaces.hh"

template< class T >
__forceinline void V_swap( T& x, T& y ) {
    T temp = x;
    x = y;
    y = temp;
}

template< class T >
inline T* Construct( T* pMemory ) {
    // return reinterpret_cast<T*>(::new( pMemory ) T);

    T ( asd ) = T( );
    return &asd;
}

template< class T >
inline void Destruct( T* pMemory ) {
    pMemory->~T( );
}


template <class T>
inline T* CopyConstruct( T* pMemory, T const& src )
{
    return ::new( pMemory ) T(src);
}

template <class T>
inline T* Copy_New( T const& src ) {
    const auto alloced = interfaces::memalloc->alloc( sizeof( T ) );
    return CopyConstruct( static_cast< T* >( alloced ), src );
}

#define V_memmove(dest, src, count)		memmove((dest), (src), (count))
#define Q_memmove				V_memmove

#endif //MEM_FUNCS_HH
