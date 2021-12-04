# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

configure_file("${PROJECT_SOURCE_DIR}/Config.hpp.in" "${PROJECT_BINARY_DIR}/Config.hpp")
include_directories("${PROJECT_BINARY_DIR}")

# Set the C++ standard to be usedzz
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)

# Add customs Flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Werror -Wfatal-errors -O0 -g -rdynamic")
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
    src/Geometry/Bounds.cpp
    src/Geometry/Collision.cpp
    src/Geometry/Geometry.cpp
    src/Geometry/Maths.cpp
    src/Geometry/String.cpp
    src/Geometry/Vector.cpp
    src/Geometry/Vertices.cpp

    src/Benchmark/BlockData.cpp
    src/Benchmark/Performance.cpp
    src/Benchmark/Recorder.cpp

    src/Corpses/Circle.cpp
    src/Corpses/Corpse.cpp
    src/Corpses/Polygon.cpp
    
    src/Constraints/Constraint.cpp
    src/Constraints/Link.cpp
    src/Constraints/Loose.cpp
    src/Constraints/Motor.cpp
    src/Constraints/Propulsor.cpp
    src/Constraints/Slider.cpp
    src/Constraints/Spring.cpp

    src/Structures/QuadTree.cpp
    src/Structures/QuadNode.cpp
    src/Structures/System.cpp

    src/Renderer/Draw.cpp
    src/Renderer/Shapes.cpp
    src/Renderer/EditorColorScheme.cpp
    src/Renderer/Event.cpp
    src/Renderer/Gui.cpp
    src/Renderer/GuiModule.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Converter.cpp

    src/main/main.cpp
)
set(HEADERS
    include/Geometry/Bounds.hpp
    include/Geometry/Collision.hpp
    include/Geometry/Geometry.hpp
    include/Geometry/Maths.hpp
    include/Geometry/String.hpp
    include/Geometry/Vector.hpp
    include/Geometry/Vertices.hpp

    include/Benchmark/BlockData.hpp
    include/Benchmark/Performance.hpp
    include/Benchmark/Recorder.hpp

    include/Corpses/Circle.hpp
    include/Corpses/Corpse.hpp
    include/Corpses/Polygon.hpp

    include/Constraints/Constraint.hpp
    include/Constraints/Link.hpp
    include/Constraints/Loose.hpp
    include/Constraints/Motor.hpp
    include/Constraints/Propulsor.hpp
    include/Constraints/Slider.hpp
    include/Constraints/Spring.hpp

    include/Structures/QuadTree.hpp
    include/Structures/QuadNode.hpp
    include/Structures/System.hpp

    include/Renderer/EditorColorScheme.hpp
    include/Renderer/GuiModule.hpp
    include/Renderer/Renderer.hpp
    include/Renderer/Structures.hpp
    include/Renderer/Converter.hpp

)

set(RESSOURCES 
    ../../assets/icon/ricon.o
)

set(FONTS
    assets/fonts/roboto.hpp
    assets/fonts/proggy.hpp
    assets/fonts/consolas.hpp
    assets/fonts/forkawersome.hpp
    assets/fonts/icons.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FONTS} ${RESSOURCES})
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
