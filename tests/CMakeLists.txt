# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

enable_testing()
project ("Simulatio Tests")

configure_file("${PROJECT_SOURCE_DIR}/../Config.hpp.in" "${PROJECT_BINARY_DIR}/Config.hpp")
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

file(GLOB TESTS_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)


set(SOURCES
    ../src/Corpses/Corpse.cpp
    ../src/Corpses/Circle.cpp
    ../src/Corpses/Polygon.cpp

    ../src/Constraints/Constraint.cpp
    ../src/Constraints/Link.cpp
    ../src/Constraints/Spring.cpp
    ../src/Constraints/Slider.cpp
    ../src/Constraints/Loose.cpp
    ../src/Constraints/Propulsor.cpp
    ../src/Constraints/Motor.cpp

    ../src/Geometry/Geometry.cpp
    ../src/Geometry/Vector.cpp
    ../src/Geometry/Bounds.cpp
    ../src/Geometry/Vertices.cpp
    ../src/Geometry/Maths.cpp
    ../src/Geometry/String.cpp
    ../src/Geometry/Collision.cpp
    
    ../src/Structures/QuadTree.cpp
    ../src/Structures/QuadNode.cpp
    ../src/Structures/System.cpp

    ../src/Benchmark/BlockData.cpp
    ../src/Benchmark/Performance.cpp
    ../src/Benchmark/Recorder.cpp
)
set(HEADERS
    ../include/Corpses/Corpse.hpp
    ../include/Corpses/Circle.hpp
    ../include/Corpses/Polygon.hpp

    ../include/Constraints/Constraint.hpp
    ../include/Constraints/Link.hpp
    ../include/Constraints/Spring.hpp
    ../include/Constraints/Slider.hpp
    ../include/Constraints/Loose.hpp
    ../include/Constraints/Propulsor.hpp
    ../include/Constraints/Motor.hpp

    ../include/Geometry/Geometry.hpp
    ../include/Geometry/Vector.hpp
    ../include/Geometry/Bounds.hpp
    ../include/Geometry/Collision.hpp
    ../include/Geometry/Vertices.hpp
    ../include/Geometry/Maths.hpp
    ../include/Geometry/String.hpp

    ../include/Structures/QuadTree.hpp
    ../include/Structures/QuadNode.hpp
    ../include/Structures/System.hpp
    
    ../include/Benchmark/BlockData.hpp
    ../include/Benchmark/Performance.hpp
    ../include/Benchmark/Recorder.hpp
)

foreach(test ${TESTS_DIR})
        get_filename_component(testName ${test} NAME_WE)
        add_executable(${testName} ${test} ${SOURCES} ${HEADERS})

        set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

        add_test(${testName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testName})
endforeach(test)