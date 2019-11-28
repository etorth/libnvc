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

MESSAGE(STATUS "download reproc")
IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/reproc/reproc.zip")
    FILE(DOWNLOAD "https://github.com/DaanDeMeyer/reproc/archive/v9.0.0.zip" "${LIBNVC_3RD_PARTY_DIR}/download/reproc/reproc.zip")
    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${LIBNVC_3RD_PARTY_DIR}/download/reproc/reproc.zip
        WORKING_DIRECTORY ${LIBNVC_3RD_PARTY_DIR}/download/reproc
    )
ENDIF()

MESSAGE(STATUS "download mpack done, in ${LIBNVC_3RD_PARTY_DIR}/download/reproc")

OPTION(REPROC++ ON)
FILE(GLOB REPROC_UNTAR_DIR "${LIBNVC_3RD_PARTY_DIR}/download/reproc/reproc-*/")
ADD_SUBDIRECTORY(${REPROC_UNTAR_DIR} ${REPROC_UNTAR_DIR}/build)
