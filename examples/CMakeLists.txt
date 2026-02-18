# Helper function for creating example executables
function(add_mpfi_example example_name source_file)
    add_executable(${example_name} ${source_file})
    
    target_link_libraries(${example_name} PRIVATE
        mpfi_agm
    )
endfunction()



# Add all examples
add_mpfi_example(demo_steps demo_steps.c)
