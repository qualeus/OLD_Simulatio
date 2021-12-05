#ifndef Window_HPP
#define Window_HPP

#include <GLFW/glfw3.h>

#include <iostream>

namespace drw {

class Window {
   private:
    GLFWwindow* window;
    int width;
    int height;

   public:
    Window();
    void Render();
    void Close();
};

}  // namespace drw
#endif