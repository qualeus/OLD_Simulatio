#ifndef Overlay_HPP
#define Overlay_HPP

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <string>

namespace ovl {

class Overlay {
   private:
    GLFWwindow* window;
    std::string glsl_version;

   public:
    Overlay(GLFWwindow* window, std::string glsl_version);
    void Initialize();
    void AddFont();
    void Prepare();
    void Render();
    void RenderDrawData();
    void Cleanup();
};

}  // namespace ovl
#endif