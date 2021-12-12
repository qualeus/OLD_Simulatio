cmake_minimum_required(VERSION 3.12)

include(FetchContent)

SET(BUILD_SHARED_LIBS OFF CACHE BOOL "")

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(cereal
  GIT_REPOSITORY https://github.com/USCiLab/cereal
  GIT_TAG 02eace19a99ce3cd564ca4e379753d69af08c2c8
)

FetchContent_GetProperties(cereal)
if(NOT cereal_POPULATED)
  FetchContent_Populate(cereal)
  add_subdirectory(${cereal_SOURCE_DIR} ${cereal_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

