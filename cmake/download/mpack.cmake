#=======================================================================================
#
#        Filename: mpack.cmake
#         Created: 05/03/2018 13:19:07
#     Description: required: LIBNVC_3RD_PARTY_DIR
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

MESSAGE(STATUS "download mpack")
IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/mpack/mpack.h")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mpack-amalgamation-v1.0/master/mpack.h" "${LIBNVC_3RD_PARTY_DIR}/download/mpack/mpack.h")
ENDIF()

IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/mpack/mpack.c")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mpack-amalgamation-v1.0/master/mpack.c" "${LIBNVC_3RD_PARTY_DIR}/download/mpack/mpack.c")
ENDIF()

MESSAGE(STATUS "download mpack done, in ${LIBNVC_3RD_PARTY_DIR}/download/mpack")

ADD_DEFINITIONS(-DMPACK_DEBUG)
ADD_DEFINITIONS(-DMPACK_STDIO)
ADD_DEFINITIONS(-DMPACK_STRINGS)
ADD_DEFINITIONS(-DMPACK_EXTENSIONS)
ADD_DEFINITIONS(-DMPACK_READ_TRACKING)
ADD_DEFINITIONS(-DMPACK_WRITE_TRACKING)

SET(MPACK_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/download/mpack")
SET(MPACK_SRC_DIR      "${LIBNVC_3RD_PARTY_DIR}/download/mpack")

INCLUDE_DIRECTORIES(SYSTEM ${MPACK_INCLUDE_DIRS})
