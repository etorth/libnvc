#=======================================================================================
#
#        Filename: BuildMSGPackC.cmake
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

INCLUDE(ExternalProject)

ExternalProject_Add(
    msgpack-c

    GIT_REPOSITORY "https://github.com/msgpack/msgpack-c.git"
    GIT_TAG        "master"
  
    SOURCE_DIR "${LIBNVC_3RD_PARTY_DIR}/msgpack-c"

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
)

SET(MSGPACKC_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/msgpack-c/include")
INCLUDE_DIRECTORIES(SYSTEM ${MSGPACKC_INCLUDE_DIRS})
