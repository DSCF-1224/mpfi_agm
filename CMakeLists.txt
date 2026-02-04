cmake_minimum_required(VERSION 3.10)

project(check_versions C)

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
