INCLUDE(FetchContent)

FetchContent_Declare(
  reproc
  GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
  GIT_TAG v14.2.5
)

SET(REPROC++ ON)
SET(REPROC_OBJECT_LIBRARIES ON)

FetchContent_MakeAvailable(reproc)
