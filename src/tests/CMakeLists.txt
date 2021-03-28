# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

enable_testing()
project ("Tests")

configure_file("../../Config.hpp.in" "${PROJECT_BINARY_DIR}/Config.hpp")
include_directories("${PROJECT_BINARY_DIR}")

# Set the C++ standard to be used
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set (CMAKE_BUILD_TYPE "Debug")

# Add customs Flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Werror -Wfatal-errors")

set(LIBRARIES_PATH "")

# CMake options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# SFML
set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES ON)

set(SFML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/${LIBRARIES_PATH}sfml/include)
set(SFML_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/${LIBRARIES_PATH}sfml/lib)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/${LIBRARIES_PATH}sfml/lib/cmake/SFML)

file(GLOB TESTS_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)


set(SOURCES
    ../Corpses/Corpse.cpp
    ../Corpses/Circle.cpp
    ../Corpses/Polygon.cpp
    ../Constraints/Constraint.cpp
    ../Constraints/Dot.cpp
    ../Geometry/Geometry.cpp
    ../Geometry/Vector.cpp
    ../Geometry/Bounds.cpp
    ../Geometry/Vertices.cpp
    ../Geometry/Text.cpp
    ../Geometry/String.cpp
    ../Geometry/Maths.cpp
    ../Structures/QuadTree.cpp
    ../Structures/QuadNode.cpp
    ../Geometry/Collision.cpp
)
set(HEADERS
    ../../include/Corpses/Corpse.hpp
    ../../include/Corpses/Circle.hpp
    ../../include/Corpses/Polygon.hpp
    ../../include/Constraints/Constraint.hpp
    ../../include/Constraints/Dot.hpp
    ../../include/Geometry/Geometry.hpp
    ../../include/Geometry/Vector.hpp
    ../../include/Geometry/Bounds.hpp
    ../../include/Structures/QuadTree.hpp
    ../../include/Structures/QuadNode.hpp
    ../../include/Geometry/Collision.hpp
    ../../include/Geometry/Vertices.hpp
    ../../include/Geometry/Text.hpp
    ../../include/Geometry/Maths.hpp
    ../../include/Geometry/String.hpp
    ../../include/System.hpp
)


set (SFML_LIBRARIES sfml-graphics sfml-window sfml-system )
link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5 COMPONENTS graphics system window)

foreach(test ${TESTS_DIR})
        get_filename_component(testName ${test} NAME_WE)
        add_executable(${testName} ${test} ${SOURCES} ${HEADERS})

        set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

        target_link_libraries(${testName} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

        add_test(${testName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testName})
endforeach(test)