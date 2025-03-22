# EpochCoreCPM.cmake
#
# This is a helper file to include EpochCore via CPM.cmake
# Usage: include(EpochCoreCPM.cmake)

set(EPOCH_CORE_REPOSITORY "https://github.com/EPOCHDevs/EpochCore.git" CACHE STRING "EpochCore repository URL")
set(EPOCH_CORE_TAG "main" CACHE STRING "EpochCore Git tag to use")

if(NOT TARGET EpochCore::epoch_core)
    include(CPM)
    CPMAddPackage(
        NAME EpochCore
        GIT_REPOSITORY ${EPOCH_CORE_REPOSITORY}
        GIT_TAG ${EPOCH_CORE_TAG}
    )
endif() 