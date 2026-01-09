INCLUDE(ExternalProject)

ExternalProject_Add(
    utf8cpp

    GIT_REPOSITORY "https://github.com/nemtrif/utfcpp.git"
    GIT_TAG        "master"

    SOURCE_DIR "${LIBNVC_3RD_PARTY_DIR}/utf8cpp"

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
)

SET(UTF8CPP_INCLUDE_DIRS "${LIBNVC_3RD_PARTY_DIR}/utf8cpp/source")
INCLUDE_DIRECTORIES(SYSTEM ${UTF8CPP_INCLUDE_DIRS})
