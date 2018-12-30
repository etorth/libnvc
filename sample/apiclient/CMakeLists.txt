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

PROJECT(apiclient)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

IF(COMPILER_SUPPORTS_CXX17)
    SET(CMAKE_CXX_STANDARD 17)
ELSE()
    MESSAGE(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support.")
ENDIF()

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

SET(CMAKE_THREAD_PREFER_PTHREAD ON)
FIND_PACKAGE(Threads REQUIRED)

ADD_SUBDIRECTORY(src)
