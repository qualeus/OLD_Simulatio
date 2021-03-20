project ("Tests")

configure_file("../../Config.hpp.in" "${PROJECT_BINARY_DIR}/Config.hpp")

# SFML
set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES ON)

set(SFML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/${LIBRARIES_PATH}sfml/include)
set(SFML_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/${LIBRARIES_PATH}sfml/lib)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/${LIBRARIES_PATH}sfml/lib/cmake/SFML)

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5 COMPONENTS graphics system window)


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
    ../Geometry/Maths.cpp
    ../Structures/OldQuadtree.cpp
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
    ../../include/Structures/OldQuadtree.hpp
    ../../include/Geometry/Collision.hpp
    ../../include/Geometry/Vertices.hpp
    ../../include/Geometry/Text.hpp
    ../../include/Geometry/Maths.hpp
    ../../include/System.hpp
)


foreach(test ${TESTS_DIR})
        get_filename_component(testName ${test} NAME_WE)
        add_executable(${testName} ${test} ${SOURCES} ${HEADERS})

        set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/test)

        target_link_libraries(${testName} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

        add_test(NAME ${testName} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/${testName})
endforeach(test)