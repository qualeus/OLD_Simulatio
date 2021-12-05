#ifndef Window_HPP
#define Window_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <iostream>
#include <string>

#include "../Overlay/Overlay.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

namespace drw {

class Window {
   private:
    GLFWwindow* window;
    drw::Camera camera;

    ovl::Overlay* overlay;

    glm::ivec2 size;
    std::string title;
    std::string glsl_version;

   public:
    Window(int width = 640, int height = 480, std::string title = "Window");
    void Render();

    void Clear();
    void Draw();
    void Prepare();

    void Close();
    void Cleanup();
    bool isOpen();

    int get_width();
    int get_height();
};

}  // namespace drw
#endif