# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the project name and version
set (PROJECT_RENDERER_NAME ${PROJECT_NAME}_Renderer)

project (${PROJECT_RENDERER_NAME})

message("Building ${PROJECT_RENDERER_NAME}...")

add_subdirectory(libs)

# Compile Target

set(SOURCES
    src/Context/Renderer.cpp
    src/Context/Window.cpp
    src/Context/Inputs.cpp

    src/Drawing/Camera.cpp
    src/Drawing/Vertex.cpp
    src/Drawing/Shader.cpp

    src/Overlay/Overlay.cpp
    src/Overlay/ImGuiHelper.cpp

    src/Overlay/Gui/GuiManager.cpp
    src/Overlay/Gui/GuiTheme.cpp
    src/Overlay/Gui/GuiModule.cpp
    
    src/Overlay/Gui/Windows/GuiMenu.cpp
    src/Overlay/Gui/Windows/GuiSideBar.cpp
    src/Overlay/Gui/Windows/GuiTimeBar.cpp
    src/Overlay/Gui/Windows/GuiPopupClearSystem.cpp

    src/Serialization/FileManager.cpp
    src/Serialization/Serialization.cpp
)

set(HEADERS
    include/Context/Renderer.hpp
    include/Context/Window.hpp
    include/Context/Inputs.hpp

    include/Drawing/Camera.hpp
    include/Drawing/Vertex.hpp
    include/Drawing/Shader.hpp

    include/Overlay/Overlay.hpp
    include/Overlay/ImGuiHelper.hpp

    include/Overlay/Gui/GuiManager.hpp
    include/Overlay/Gui/GuiTheme.hpp
    include/Overlay/Gui/GuiModule.hpp

    include/Serialization/FileManager.hpp
    include/Serialization/Serialization.hpp
)

set(RESSOURCES 
    assets/shaders/imgui/fs_imgui.bin.hpp
    assets/shaders/imgui/vs_imgui.bin.hpp
    assets/icon/ricon.hpp
)

set(FONTS
    assets/fonts/roboto.hpp
    assets/fonts/proggy.hpp
    assets/fonts/consolas.hpp
    assets/fonts/forkawersome.hpp
    assets/fonts/icons.hpp
)

add_library(${PROJECT_RENDERER_NAME} STATIC ${SOURCES} ${HEADERS} ${RESSOURCES} ${FONTS})
target_include_directories(${PROJECT_RENDERER_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# add_subdirectory(examples)
add_subdirectory(assets/shaders)

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
target_link_libraries (${PROJECT_RENDERER_NAME} PUBLIC cereal)

