# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name and version
set (PROJECT_RENDERER_NAME ${PROJECT_NAME}_Renderer)

project (${PROJECT_RENDERER_NAME})

message("Building ${PROJECT_RENDERER_NAME}...")

add_subdirectory(libs)

# Compile Target

set(SOURCES
    src/Drawing/Window.cpp
    src/Drawing/Camera.cpp
    src/Drawing/Shader.cpp
    src/Drawing/Mesh.cpp
    src/Drawing/Vertex.cpp

    src/Overlay/Overlay.cpp
    src/Overlay/ImGuiBinder.cpp
    src/Overlay/ImGuiHelper.cpp

    src/Renderer/Renderer.cpp
)

set(HEADERS
    include/Drawing/Window.hpp
    include/Drawing/Camera.hpp
    include/Drawing/Shader.hpp
    include/Drawing/Mesh.hpp
    include/Drawing/Vertex.hpp

    include/Overlay/Overlay.hpp
    include/Overlay/ImGuiBinder.hpp
    include/Overlay/ImGuiHelper.hpp
    include/Overlay/fs_ocornut_imgui.bin.h
    include/Overlay/vs_ocornut_imgui.bin.h

    include/Renderer/Renderer.hpp
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

add_library(${PROJECT_RENDERER_NAME} STATIC ${SOURCES} ${HEADERS} ${RESSOURCES} ${FONTS})
target_include_directories(${PROJECT_RENDERER_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# add_subdirectory(examples)

if(EMSCRIPTEN)
  target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC "-s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC "-s USE_WEBGL2=1")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
  target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC glfw)
  target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC libglew_static)
endif()

target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC glm)
target_link_libraries (${PROJECT_RENDERER_NAME} PRIVATE freetype)
target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC imgui)
target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC bgfx)
