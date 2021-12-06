#ifndef Overlay_HPP
#define Overlay_HPP

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <string>

#include "ImGuiBinder.hpp"

namespace ovl {

class Overlay {
   private:
   public:
    Overlay();

    void Initialize(int view, GLFWwindow* window, std::string glsl_version);
    void Prepare();
    void Render();
    void RenderDraw();
    void Cleanup();
};

}  // namespace ovl
#endif