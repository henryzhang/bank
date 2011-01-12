#ifndef BANK_DETAIL_PLATFORM_HPP
#define BANK_DETAIL_PLATFORM_HPP

#if defined(_MSC_VER)
    #define BANK_WINDOWS_PLATFORM
#elif defined(__FreeBSD__)
    #define BANK_FREEBSD_PLATFORM
    #define BANK_GENBSD_PLATFORM
#elif defined(__APPLE__)
    #define BANK_MACOSX_PLATFORM
    #define BANK_GENBSD_PLATFORM
#elif defined(__linux) || defined(linux)
    #define BANK_LINUX_PLATFORM
#else
    #error "Unknown Platform :("
#endif

#endif /* BANK_DETAIL_PLATFORM_HPP */
