#ifndef BANK_DETAIL_TYPES_HPP
#define BANK_DETAIL_TYPES_HPP


#if !defined(_MSC_VER)
    #include <stdint.h>
namespace bank {
    typedef ::int8_t int8_t;
    typedef ::int16_t int16_t;
    typedef ::int32_t int32_t;
    typedef ::int64_t int64_t;

    typedef ::uint8_t uint8_t;
    typedef ::uint16_t uint16_t;
    typedef ::uint32_t uint32_t;
    typedef ::uint64_t uint64_t;

#else
namespace bank {

    typedef signed __int8 int8_t;
    typedef signed __int16 int16_t;
    typedef signed __int32 int32_t;
    typedef signed __int64 int64_t;

    typedef unsigned __int8 uint8_t;
    typedef unsigned __int16 uint16_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;

    #if defined(_WIN64)
        typedef signed __int64 ssize_t;
    #else
        typedef signed __int32 ssize_t;
    #endif /* _WIN64 */
#endif /* _MSC_VER */

} /* namespace bank */

#endif /* BANK_TYPES_HPP */
