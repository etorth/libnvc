#=======================================================================================
#
#        Filename: strfunc.cmake
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

FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/strfunc.hpp" "${LIBNVC_3RD_PARTY_DIR}/download/strfunc")
FILE(DOWNLOAD "https://raw.githubusercontent.com/etorth/mir2x/master/common/src/strfunc.cpp" "${LIBNVC_3RD_PARTY_DIR}/download/strfunc")
