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

INCLUDE(FetchContent)

FetchContent_Declare(
  reproc
  GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
  GIT_TAG 0a2865af200bb1bc5bb7f5140383c3f51ebc6d13
)

SET(REPROC++ ON)
SET(REPROC_OBJECT_LIBRARIES ON)

FetchContent_MakeAvailable(reproc)
