# EpochCore

A header-only C++ library providing core utilities and interfaces.

## Integration with CPM

EpochCore can be easily integrated into your project using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake).

### Option 1: Direct inclusion in your CMakeLists.txt

```cmake
# Add CPM.cmake (if not already added)
include(cmake/CPM.cmake)
# Or download it if you don't have it
# set(CPM_DOWNLOAD_VERSION 0.38.1)
# get_filename_component(CPM_DOWNLOAD_LOCATION "${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake" ABSOLUTE)
# if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
#     message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
#     file(DOWNLOAD
#         https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
#         ${CPM_DOWNLOAD_LOCATION}
#     )
# endif()
# include(${CPM_DOWNLOAD_LOCATION})

# Add EpochCore
CPMAddPackage(
    NAME EpochCore
    GIT_REPOSITORY https://github.com/EPOCHDevs/EpochCore.git
    GIT_TAG main
)

# Link your target
target_link_libraries(your_target PRIVATE epoch::core)
```

### Option 2: Using the helper file

Alternatively, you can use the provided helper file:

1. Copy `cmake/EpochCoreCPM.cmake` to your project's cmake directory
2. Include it in your CMakeLists.txt:

```cmake
include(cmake/EpochCoreCPM.cmake)

# Link your target
target_link_libraries(your_target PRIVATE epoch::core)
```

## Available Headers

The library provides the following header files:

- `error_context.h` - Error context handling
- `error_type.h` - Error type definitions
- `status.h` - Status return values
- `common_utils.h` - Common utility functions
- `endianness.h` - Endianness handling
- `uuid_v4.h` - UUID v4 generation
- `catch_defs.h` - Catch2 helper definitions
- `macros.h` - Useful macros
- `enum_wrapper.h` - Enum wrapper utilities
- `ranges_to.h` - Range utilities 