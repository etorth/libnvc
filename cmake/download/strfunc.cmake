#=======================================================================================
#
#        Filename: strf.cmake
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

MESSAGE(STATUS "download strf")
IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/strf/strf.hpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/strf.hpp" "${LIBNVC_3RD_PARTY_DIR}/download/strf/strf.hpp")
ENDIF()

IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/strf/strf.cpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/strf.cpp" "${LIBNVC_3RD_PARTY_DIR}/download/strf/strf.cpp")
ENDIF()

IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/strf/fflerror.hpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/fflerror.hpp" "${LIBNVC_3RD_PARTY_DIR}/download/strf/fflerror.hpp")
ENDIF()
MESSAGE(STATUS "download strf done, in ${LIBNVC_3RD_PARTY_DIR}/download/strf")

SET(STRFUNC_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/download/strf")
SET(STRFUNC_SRC_DIR      "${LIBNVC_3RD_PARTY_DIR}/download/strf")

INCLUDE_DIRECTORIES(SYSTEM ${STRFUNC_INCLUDE_DIRS})
