INCLUDE(ExternalProject)

ExternalProject_Add(
    asio

    PREFIX "${LIBNVC_3RD_PARTY_DIR}/asio"
    URL https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-36-0.tar.gz

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
)

ADD_DEFINITIONS(-DASIO_STANDALONE)
SET(ASIO_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/asio/src/asio/asio/include")
INCLUDE_DIRECTORIES(SYSTEM ${ASIO_INCLUDE_DIRS})
