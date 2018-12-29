#=======================================================================================
#
#        Filename: BuildMPack.cmake
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
    mpack

    PREFIX ${LIBNVC_3RD_PARTY_DIR}/mpack
    URL https://github.com/ludocode/mpack/releases/download/v1.0/mpack-amalgamation-1.0.tar.gz

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
)

# did I made anything wrong?
# I don't know why the dir name is so long?

SET(MPACK_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/mpack/src/mpack/src/mpack")
SET(MPACK_SRC_DIR ${MPACK_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(SYSTEM ${MPACK_INCLUDE_DIRS})
