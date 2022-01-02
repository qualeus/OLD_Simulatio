#ifndef Overlay_HPP
#define Overlay_HPP

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <imgui.h>

#include <Geometry/String.hpp>
#include <stdexcept>
#include <string>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <bgfx/platform.h>

#include "../../assets/shaders/imgui/fs_imgui.bin.hpp"
#include "../../assets/shaders/imgui/vs_imgui.bin.hpp"
#include "../Context/Inputs.hpp"
#include "ImGuiHelper.hpp"

namespace ovl {

class Overlay {
   private:
    const bgfx::Memory* get_vs_shader();
    const bgfx::Memory* get_fs_shader();
    bgfx::ProgramHandle create_program(const char* name);

   public:
    Overlay();

    void Initialize(GLFWwindow* window);
    void Draw();
    void PollEvents(float dt);
    void Reset(int width, int height);
    void Cleanup();

    void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void glfw_char_callback(GLFWwindow* window, unsigned int codepoint);
    void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

}  // namespace ovl
#endif