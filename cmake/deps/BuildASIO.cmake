#=======================================================================================
#
#        Filename: BuildASIO.cmake
#         Created: 05/03/2016 13:19:07
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

INCLUDE(ExternalProject)

ExternalProject_Add(
    asio

    PREFIX "${LIBNVC_3RD_PARTY_DIR}/asio"
    URL https://github.com/chriskohlhoff/asio/archive/asio-1-12-2.tar.gz

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
)

ADD_DEFINITIONS(-DASIO_STANDALONE)
SET(ASIO_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/asio/src/asio/asio/include")
INCLUDE_DIRECTORIES(SYSTEM ${ASIO_INCLUDE_DIRS})
