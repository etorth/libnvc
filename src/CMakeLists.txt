AUX_SOURCE_DIRECTORY(. LIBNVC_SRC)
ADD_LIBRARY(nvc ${LIBNVC_SRC})

# this is an interface lib
# don't export any unnecessary dependency
TARGET_INCLUDE_DIRECTORIES(nvc PUBLIC "${LIBNVC_INCLUDE_DIR}")
