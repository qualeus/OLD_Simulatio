#ifndef Window_HPP
#define Window_HPP

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "../Overlay/Overlay.hpp"

namespace drw {

class Window {
   private:
    GLFWwindow* window;
    ovl::Overlay* overlay;
    int width;
    int height;
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
};

}  // namespace drw
#endif