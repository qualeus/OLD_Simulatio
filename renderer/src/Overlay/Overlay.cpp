#include "../../include/Overlay/Overlay.hpp"

namespace ovl {

Overlay::Overlay(GLFWwindow* window, std::string glsl_version) {
    this->window = window;
    this->glsl_version = glsl_version;
}

void Overlay::Initialize() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init(this->glsl_version.c_str());
}

void Overlay::AddFont() {}

void Overlay::Prepare() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Overlay::Render() { ImGui::Render(); }
void Overlay::RenderDrawData() { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }
void Overlay::Cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

}  // namespace ovl