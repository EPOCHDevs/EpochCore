if(TARGET epoch::core)
    return()
endif()

add_library(epoch::core INTERFACE IMPORTED)
set_target_properties(epoch::core PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EPOCH_CORE_INCLUDE_DIR}"
) 