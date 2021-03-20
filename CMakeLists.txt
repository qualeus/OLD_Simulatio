# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name and version
set (PROJECT_NAME "PHYSICS")
set (PROJECT_VERSION_MAJOR 0)
set (PROJECT_VERSION_MINOR 5)
set (PROJECT_VERSION_REVISION 2)
project (${PROJECT_NAME} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_REVISION})

message("PROJECT ${PROJECT_NAME} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_REVISION}")

# Set the project Config variables
set (ENABLE_DOUBLE_PRECISION ON)
# add_definitions( -DENABLE_DOUBLE_PRECISION=${ENABLEs_DOUBLE_PRECISION} )

configure_file("${PROJECT_SOURCE_DIR}/Config.hpp.in" "${PROJECT_BINARY_DIR}/Config.hpp")

# Set the C++ standard to be used
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    src/main/main.cpp
    src/Corpses/Corpse.cpp
    src/Corpses/Circle.cpp
    src/Corpses/Polygon.cpp
    src/Constraints/Constraint.cpp
    src/Constraints/Dot.cpp
    src/Geometry/Geometry.cpp
    src/Geometry/Vector.cpp
    src/Geometry/Bounds.cpp
    src/Geometry/Vertices.cpp
    src/Geometry/Text.cpp
    src/Geometry/Maths.cpp
    src/Structures/OldQuadtree.cpp
    src/Geometry/Collision.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Draw.cpp
    src/Renderer/Event.cpp
    src/Renderer/Gui.cpp
    src/Renderer/GuiModule.cpp
    src/System.cpp 
)
set(HEADERS
    include/Corpses/Corpse.hpp
    include/Corpses/Circle.hpp
    include/Corpses/Polygon.hpp
    include/Constraints/Constraint.hpp
    include/Constraints/Dot.hpp
    include/Geometry/Geometry.hpp
    include/Geometry/Vector.hpp
    include/Geometry/Bounds.hpp
    include/Structures/OldQuadtree.hpp
    include/Geometry/Collision.hpp
    include/Geometry/Vertices.hpp
    include/Geometry/Text.hpp
    include/Geometry/Maths.hpp
    include/Renderer/Renderer.hpp
    include/Renderer/GuiModule.hpp
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

# Build tests only in Debug mode
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(src/tests)
endif()
# Copy assets
#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})