#=======================================================================================
#
#        Filename: reproc.cmake
#         Created: 05/03/2016 13:19:07
#     Description: required: NVIM_SDL2_3RD_PARTY_DIR
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

INCLUDE(ExternalProject)

ExternalProject_Add(
    reproc

    GIT_REPOSITORY "https://github.com/DaanDeMeyer/reproc"
    GIT_TAG        "master"
  
    SOURCE_DIR "${NVIM_SDL2_3RD_PARTY_DIR}/reproc"
    INSTALL_DIR "${NVIM_SDL2_3RD_PARTY_DIR}/reproc/build"

    UPDATE_COMMAND ""
    PATCH_COMMAND ""

    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${NVIM_SDL2_3RD_PARTY_DIR}/reproc/build/install -DREPROC++=ON

    LOG_BUILD 1
    LOG_CONFIGURE 1
    LOG_INSTALL 1
)

SET(REPROC_INCLUDE_DIRS "${NVIM_SDL2_3RD_PARTY_DIR}/reproc/build/install/include")
SET(REPROC_LIBRARIES    "${CMAKE_STATIC_LIBRARY_PREFIX}reproc${CMAKE_STATIC_LIBRARY_SUFFIX}")
SET(REPROCXX_LIBRARIES  "${CMAKE_STATIC_LIBRARY_PREFIX}reproc++${CMAKE_STATIC_LIBRARY_SUFFIX}")

INCLUDE_DIRECTORIES(SYSTEM ${REPROC_INCLUDE_DIRS})
LINK_DIRECTORIES(${NVIM_SDL2_3RD_PARTY_DIR}/reproc/build/install/lib)
