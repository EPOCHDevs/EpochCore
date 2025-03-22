# EpochCoreCPM.cmake
#
# This is a helper file to include EpochCore via CPM.cmake
# Usage: include(EpochCoreCPM.cmake)

set(EPOCH_CORE_REPOSITORY "https://github.com/EPOCHDevs/EpochCore.git" CACHE STRING "EpochCore repository URL")
set(EPOCH_CORE_TAG "master" CACHE STRING "EpochCore Git tag to use")

if(NOT TARGET epoch::core)
    # Download CPM.cmake if not already available
    set(CPM_DOWNLOAD_VERSION 0.38.7)
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    
    if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
        message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
        file(DOWNLOAD
            https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION}
        )
    endif()
    
    include(${CPM_DOWNLOAD_LOCATION})
    
    CPMAddPackage(
        NAME EpochCore
        GIT_REPOSITORY ${EPOCH_CORE_REPOSITORY}
        GIT_TAG ${EPOCH_CORE_TAG}
    )
endif() 