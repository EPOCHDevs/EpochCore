cmake_minimum_required(VERSION 3.10)
project(EpochCore VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Create a header-only interface library
add_library(epoch_core INTERFACE)

# Set include directories
target_include_directories(epoch_core
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Export targets for CPM integration
set(EPOCH_CORE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "")
add_library(EpochCore::epoch_core ALIAS epoch_core)
