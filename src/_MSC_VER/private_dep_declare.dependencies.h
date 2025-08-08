
#if defined(_MSC_VER)
    #include <BaseTsd.h>
    #include <intrin.h>

    typedef SSIZE_T ssize_t;
    typedef __m128i __m128i_u;

    #define __src_inner_hbyteswap_ulong _byteswap_ulong
#endif
