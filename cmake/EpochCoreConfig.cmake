if(TARGET EpochCore::epoch_core)
    return()
endif()

add_library(EpochCore::epoch_core INTERFACE IMPORTED)
set_target_properties(EpochCore::epoch_core PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EPOCH_CORE_INCLUDE_DIR}"
) 