AUX_SOURCE_DIRECTORY(. APICLIENT_SRC)
ADD_EXECUTABLE(apiclient ${APICLIENT_SRC})

TARGET_INCLUDE_DIRECTORIES(apiclient PRIVATE ${LIBNVC_INCLUDE})
FIND_LIBRARY(LIBNVC_LIBNAME nvc HINTS ${LIBNVC_LIB})

# for the pthread issue
# from: https://stackoverflow.com/questions/1620918/cmake-and-libpthread
TARGET_LINK_LIBRARIES(apiclient Threads::Threads)
TARGET_LINK_LIBRARIES(apiclient ${LIBNVC_LIBNAME})

INSTALL(TARGETS apiclient DESTINATION apiclient)
