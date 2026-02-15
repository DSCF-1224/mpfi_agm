cmake_minimum_required(VERSION 3.10)

project(check_versions C)



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
# It's typically installed alongside MPFR
find_library(MPFI_LIBRARY NAMES mpfi REQUIRED)
find_path(MPFI_INCLUDE_DIR mpfi.h REQUIRED)



# Create executable: Version checker
add_executable(check_versions tests/check_versions.c)

# Link libraries
target_link_libraries(check_versions 
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
    ${MPFI_LIBRARY}
)

# Include directories
target_include_directories(check_versions PRIVATE
    ${GMP_INCLUDE_DIRS}
    ${MPFR_INCLUDE_DIRS}
    ${MPFI_INCLUDE_DIR}
)



# Create executable: test
add_executable(check_nan tests/check_nan.c)

# Link libraries
target_link_libraries(check_nan 
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
    ${MPFI_LIBRARY}
)

# Include directories
target_include_directories(check_nan PRIVATE
    ${GMP_INCLUDE_DIRS}
    ${MPFR_INCLUDE_DIRS}
    ${MPFI_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
)



# Create executable: test
add_executable(check_step tests/check_step.c)

# Link libraries
target_link_libraries(check_step 
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
    ${MPFI_LIBRARY}
)

# Include directories
target_include_directories(check_step PRIVATE
    ${GMP_INCLUDE_DIRS}
    ${MPFR_INCLUDE_DIRS}
    ${MPFI_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
)



# Create executable: example
add_executable(demo_steps examples/demo_steps.c)

# Link libraries
target_link_libraries(demo_steps 
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
    ${MPFI_LIBRARY}
)

# Include directories
target_include_directories(demo_steps PRIVATE
    ${GMP_INCLUDE_DIRS}
    ${MPFR_INCLUDE_DIRS}
    ${MPFI_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
)
