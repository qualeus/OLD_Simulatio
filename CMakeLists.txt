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

#set(LIBRARIES_PATH "Windows/g++/Release/")
set(LIBRARIES_PATH "")

# CMake options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set default build type explicitly to Release
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build type." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# SFML
set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES ON)

set(SFML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/${LIBRARIES_PATH}sfml/include)
set(SFML_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/${LIBRARIES_PATH}sfml/lib)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/${LIBRARIES_PATH}sfml/lib/cmake/SFML)

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5 COMPONENTS graphics system window)

# IMGUI
set(ImGui-SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/${LIBRARIES_PATH}imgui/lib/cmake/ImGui-SFML)

find_package(ImGui-SFML REQUIRED)

# Compile Target

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
    assets/fonts/roboto.hpp
    assets/fonts/proggy.hpp
    assets/fonts/consolas.hpp
    assets/fonts/iconsforkawesome.cpp
    assets/fonts/IconsForkAwesome.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FONTS})
set (SFML_LIBRARIES sfml-graphics sfml-window sfml-system )
set (IMGUI_LIBRARIES ImGui-SFML::ImGui-SFML)

# Link libraries

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${IMGUI_LIBRARIES} -lmingw32 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid)
else()
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${IMGUI_LIBRARIES})
endif()
# Copy assets
#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})