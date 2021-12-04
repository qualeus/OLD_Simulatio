# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name and version
set (PROJECT_NAME "Simulatio")
set (PROJECT_VERSION_MAJOR 0)
set (PROJECT_VERSION_MINOR 5)
set (PROJECT_VERSION_REVISION 2)

project (${PROJECT_NAME} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_REVISION})

message("Building ${PROJECT_NAME} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_REVISION}...")

# Configure dependencies for build static libraries only. 
set(BUILD_SHARED_LIBS OFF)

# Set the C++ standard to be used
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)

# Add customs Flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Werror -Wfatal-errors -O0 -g -rdynamic")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Werror -Wfatal-errors -O3 -s -fexpensive-optimizations")

# CMake options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Add as many warning as possible:
if (MSVC)
  # target_compile_options(smk PRIVATE "/wd4244")
  # target_compile_options(smk PRIVATE "/wd4267")
  # target_compile_options(smk PRIVATE "/wd4996")
  # target_compile_options(smk PRIVATE "/wd4305")
else()
  # target_compile_options(smk PRIVATE "-Wall")
  # target_compile_options(smk PRIVATE "-Werror")
  # target_compile_options(smk PRIVATE "-Wno-sign-compare")
  # target_compile_options(smk PRIVATE "-Wno-return-type")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set default build type explicitly to Release
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build type." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

add_subdirectory(engine)
add_subdirectory(renderer)