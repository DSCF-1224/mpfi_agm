cmake_minimum_required(VERSION 3.10)

project(mpfi_agm LANGUAGES C)



if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "`CMAKE_BUILD_TYPE` not specified")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -O0 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -O3")
else()
    message(FATAL_ERROR "Invalid `CMAKE_BUILD_TYPE`: ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "CMAKE_BUILD_TYPE      : ${CMAKE_BUILD_TYPE}"      )
message(STATUS "CMAKE_C_FLAGS         : ${CMAKE_C_FLAGS}"         )
message(STATUS "CMAKE_C_FLAGS_DEBUG   : ${CMAKE_C_FLAGS_DEBUG}"   )
message(STATUS "CMAKE_C_FLAGS_RELEASE : ${CMAKE_C_FLAGS_RELEASE}" )



# Find GMP and MPFR using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED gmp)
pkg_check_modules(MPFR REQUIRED mpfr)

# MPFI doesn't have pkg-config, so we find it manually
find_library(MPFI_LIBRARY NAMES mpfi REQUIRED)
find_path(MPFI_INCLUDE_DIR mpfi.h REQUIRED)



# Compiled library target for mpfi_agm
add_library(mpfi_agm src/mpfi_agm.c)

target_include_directories(mpfi_agm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mpfi_agm PRIVATE
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
    ${MPFI_LIBRARY}
)



# Option to build tests and examples
option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
