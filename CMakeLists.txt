# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name
set (PROJECT_NAME "physics")
project (${PROJECT_NAME})

# Set the C++ standard to be used
set (CMAKE_CXX_STANDARD 17)

# Add customs Flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Werror -Wfatal-errors")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Werror -Wfatal-errors -O3 -s -fexpensive-optimizations")

# CMake options
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set default build type explicitly to Release
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build type." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# SFML
set(SFML_DIR "D:/Librairies/sfml_2_5_1/sfml_2_5_1_build/lib/cmake/SFML")
set(SFML_DOC_DIR "D:/Librairies/sfml_2_5_1/sfml_2_5_1_build/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
find_package (SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML couldn't be located!")
endif()

# IMGUI
set(ImGui-SFML_DIR "D:/Librairies/imgui-sfml_2_0_0/imgui_docking_build/lib/cmake/ImGui-SFML")
find_package(ImGui-SFML REQUIRED)

# Compile Target
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES
    src/main.cpp
    src/Circle.cpp
    src/Constraint.cpp
    src/Corpse.cpp
    src/Dot.cpp
    src/Functional.cpp
    src/Polygon.cpp
    src/Quadtree.cpp
    src/Renderer.cpp
    src/RendererDraw.cpp
    src/RendererEvents.cpp
    src/RendererGui.cpp
    src/System.cpp  
)
set(HEADERS
    include/Circle.hpp
    include/Constraint.hpp
    include/Corpse.hpp
    include/Dot.hpp
    include/Functional.hpp
    include/Polygon.hpp
    include/Quadtree.hpp
    include/Renderer.hpp
    include/System.hpp
)

set(FONTS
    assets/fonts/roboto.cpp
    assets/fonts/proggy.cpp
    assets/fonts/iconsforkawesome.cpp
    assets/fonts/IconsForkAwesome.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FONTS})

# Link libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system ImGui-SFML::ImGui-SFML)

# Copy assets
#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})