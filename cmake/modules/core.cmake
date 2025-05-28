# Define the reusable function
function(pretty_print_files label list_ref)
    message(STATUS "${label}:")
    foreach(item IN LISTS ${list_ref})
        message("  ${item}")
    endforeach()
endfunction()
