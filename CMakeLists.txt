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
    if (NOT ${GCC_VERSION} MATCHES ".*[4-9]\\.[5-9].*")
        find_program(GCC_REQUIRED g++4.5 ENV PATH)
        set(CMAKE_CXX_COMPILER ${GCC_REQUIRED} CACHE STRING "" FORCE)
        message(FATAL_ERROR "Requires g++ version 4.5 or later")
    endif ()
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -flto) # Adds link time optimization
endif ()

file(GLOB_RECURSE BANK_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(bank STATIC ${BANK_SOURCE})

target_link_libraries(bank synk)

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
