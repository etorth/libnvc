#=======================================================================================
#
#        Filename: extfunc.cmake
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

MESSAGE(STATUS "download extfunc")
IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/strf.hpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/strf.hpp" "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/strf.hpp")
ENDIF()

IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/strf.cpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/strf.cpp" "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/strf.cpp")
ENDIF()

IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/fflerror.hpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/fflerror.hpp" "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/fflerror.hpp")
ENDIF()

IF(NOT EXISTS "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/fileptr.hpp")
    FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/fileptr.hpp" "${LIBNVC_3RD_PARTY_DIR}/download/extfunc/fileptr.hpp")
ENDIF()

MESSAGE(STATUS "download extfunc done, in ${LIBNVC_3RD_PARTY_DIR}/download/extfunc")

SET(EXTFUNC_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/download/extfunc")
SET(EXTFUNC_SRC_DIR      "${LIBNVC_3RD_PARTY_DIR}/download/extfunc")

INCLUDE_DIRECTORIES(SYSTEM ${EXTFUNC_INCLUDE_DIRS})
