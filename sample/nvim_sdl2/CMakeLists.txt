#=======================================================================================
#
#        Filename: CMakeLists.txt
#         Created: 12/25/2018 13:19:07
#     Description:
#
#         Version: 1.0
#        Revision: none
#        Compiler: cmake
#
#          Author: ANHONG
#           Email: anhonghe@gmail.com
#    Organization: USTC
#
#=======================================================================================

PROJECT(nvim_sdl2)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

IF(COMPILER_SUPPORTS_CXX17)
    SET(CMAKE_CXX_STANDARD 17)
ELSE()
    MESSAGE(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support.")
ENDIF()

# add flags to release mode
# by default won't use release mode
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-strict-aliasing")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wfatal-errors -Wextra -Wunused -Werror")

# always compile libnvc in debug mode
# for trace message use LIBNVC_DEBUG_ARG_LIST to enable
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -pedantic")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-strict-aliasing")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wunused -Werror")

# to build this project
# user need to explicitly specify where is your libnvc include/lib

IF(DEFINED LIBNVC_INCLUDE)
    MESSAGE(STATUS "Found libnvc include path: ${LIBNVC_INCLUDE}")
ELSE()
    MESSAGE(FATAL_ERROR "Required LIBNVC_INCLUDE=\"to-libnvc-include-path\" not defined")
ENDIF()

IF(DEFINED LIBNVC_LIB)
    MESSAGE(STATUS "Found libnvc library path: ${LIBNVC_LIB}")
ELSE()
    MESSAGE(FATAL_ERROR "Required LIBNVC_LIB=\"to-libnvc-library-path\" not defined")
ENDIF()

SET(NVIM_SDL2_3RD_PARTY_DIR "${CMAKE_CURRENT_BINARY_DIR}/3rdparty")
FILE(GLOB NVIM_SDL2_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps/*.cmake")
FOREACH(NVIM_SDL2_DEP ${NVIM_SDL2_DEPS_DIR})
    INCLUDE(${NVIM_SDL2_DEP})
ENDFOREACH()

SET(CMAKE_THREAD_PREFER_PTHREAD ON)
FIND_PACKAGE(Threads REQUIRED)

ADD_SUBDIRECTORY(src)
