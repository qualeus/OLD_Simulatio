# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name and version
set (PROJECT_RENDERER_NAME ${PROJECT_NAME}_Renderer)

project (${PROJECT_RENDERER_NAME})

message("Building ${PROJECT_RENDERER_NAME}...")

add_subdirectory(libs)

# Compile Target

set(SOURCES
    src/main/main.cpp
)
set(HEADERS
)

set(RESSOURCES 
    # ../assets/icon/ricon.o
)

set(FONTS
    # ../assets/fonts/roboto.hpp
    # ../assets/fonts/proggy.hpp
    # ../assets/fonts/consolas.hpp
    # ../assets/fonts/forkawersome.hpp
    # ../assets/fonts/icons.hpp
)

# add_subdirectory(examples)

add_executable(${PROJECT_RENDERER_NAME} ${SOURCES} ${HEADERS} ${FONTS} ${RESSOURCES})

if(EMSCRIPTEN)
  target_link_libraries(${PROJECT_RENDERER_NAME} PUBLIC "-s USE_GLFW=3")
  target_link_libraries(${PROJECT_RENDERER_NAME} PUBLIC "-s USE_WEBGL2=1")
else()
  target_link_libraries(${PROJECT_RENDERER_NAME} PUBLIC glfw)
  target_link_libraries(${PROJECT_RENDERER_NAME} PUBLIC libglew_static)
endif()

target_link_libraries(${PROJECT_RENDERER_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_RENDERER_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_RENDERER_NAME} PUBLIC imgui)
