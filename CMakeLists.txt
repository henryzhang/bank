cmake_minimum_required(VERSION 2.8)

option(BUILD_EXAMPLE "Build the example application" FALSE)

if (APPLE)
    option(USE_CLANG_COMPILER "Use clang as the C++ compiler" TRUE)
    if (${USE_CLANG_COMPILER})
        find_program(CLANG clang++ ENV PATH)
        set(CMAKE_CXX_COMPILER ${CLANG} CACHE STRING "" FORCE)
    endif ()
endif ()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

project(bank)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND NOT ${USE_CLANG_COMPILER})
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (${GCC_VERSION} MATCHES ".*[4-9]\\.[5-9].*")
        set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} -flto) # Adds link time optimization
    endif ()
endif ()

# Glob file expression
if (MSVC)
    set(GLOB_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/source/win32/*.cpp)
    add_definitions("/wd4290") # It is safe to disable this warning
endif ()
if (UNIX)
    set(GLOB_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/source/posix/*.cpp)
endif ()
set(GLOB_EXPRESSION ${GLOB_EXPRESSION} ${CMAKE_CURRENT_SOURCE_DIR}/source/shared/*.cpp)

file(GLOB_RECURSE BANK_SOURCE ${GLOB_EXPRESSION})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(bank STATIC ${BANK_SOURCE})

# We depend on pthreads for POSIX
if (UNIX)
    target_link_libraries(bank pthread)
endif ()

if (BUILD_EXAMPLE)
    add_executable(bank-example ${CMAKE_CURRENT_SOURCE_DIR}/examples/bank-example.cpp)
    target_link_libraries(bank-example bank)
endif ()

# Install Information
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING PATTERN "*.hpp")

install(TARGETS bank
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
