# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name and version
set (PROJECT_ENGINE_NAME ${PROJECT_NAME}_Engine)

project (${PROJECT_ENGINE_NAME})

message("Building ${PROJECT_ENGINE_NAME}...")

add_subdirectory(libs)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
)

add_library(${PROJECT_ENGINE_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries (${PROJECT_ENGINE_NAME} PUBLIC cereal)