# Helper function for creating test executables
function(add_mpfi_test test_name source_file)
    add_executable(${test_name} ${source_file})
    
    target_link_libraries(${test_name} PRIVATE
        mpfi_agm
    )
    
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()



# Add all tests
add_mpfi_test( check_nan         check_nan.c         )
add_mpfi_test( check_negative_input check_negative_input.c )
add_mpfi_test( check_point_input check_point_input.c )
add_mpfi_test( check_step        check_step.c        )
add_mpfi_test( check_versions    check_versions.c    )
