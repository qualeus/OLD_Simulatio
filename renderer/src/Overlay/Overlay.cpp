#include "../../include/Overlay/Overlay.hpp"

namespace ovl {

// https://github.com/bkaradzic/bgfx/blob/master/examples/common/imgui/imgui.cpp
// https://github.com/Josh4428/bigg
// https://github.com/pr0g/sdl-bgfx-imgui-starter/blob/main/main.cpp
// https://github.com/pr0g/sdl-bgfx-imgui-as_1d-nonlinear-transformations/blob/main/main.cpp
// https://github.com/bkaradzic/bgfx/blob/master/examples/00-helloworld/helloworld.cpp
// https://github.com/bkaradzic/bgfx/tree/master/examples/common/imgui

Overlay::Overlay() {}

void Overlay::Initialize(int view, GLFWwindow* window, std::string glsl_version) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup inputs
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_Implbgfx_Init(255);  // view);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void Overlay::Prepare() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui_Implbgfx_NewFrame();
}

void Overlay::Render() { ImGui::Render(); }

void Overlay::RenderDraw() {
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::Cleanup() {
    ImGui_Implbgfx_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

}  // namespace ovl