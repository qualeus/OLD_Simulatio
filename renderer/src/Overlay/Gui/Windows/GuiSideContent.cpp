#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

void GuiManager::DrawGuiSideContent() {
    bool always_show = true;

    side_content_size = ImGui::AnimationLinear(show_side_content, side_content_size, 0.0f, G_SIDE_CONTENT_SIZE, G_ANIMATION_SPEED);

    const float diff_size = G_SIDE_CONTENT_SIZE - side_content_size;

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x - diff_size + side_bar_size, viewport->GetWorkPos().y));
    ImGui::SetNextWindowSize(ImVec2(G_SIDE_CONTENT_SIZE, viewport->GetWorkSize().y + side_bar_size));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[F_ROBOTO_MEDIUM]);
    if (ImGui::Begin("SideContent", &always_show,
                     ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
        ImGui::PushFont(io.Fonts->Fonts[F_ROBOTO_DEFAULT]);
        if (side_content_size != 0.0f) {
            switch (input_side_menu) {
                case I_SIDE_MENU_DEFAULT: {
                    DrawGuiSideDefault();
                } break;
                case I_SIDE_MENU_SHAPE: {
                    DrawGuiSideShape();
                } break;
                case I_SIDE_MENU_SPAWNER: {
                    DrawGuiSideSpawner();
                } break;
                default: {
                }
            }
        }
        ImGui::PopFont();
        ImGui::End();
    }
    ImGui::PopFont();
    ImGui::PopStyleVar(3);
}

void GuiManager::DrawGuiSideDefault() {
    std::string text = "Select something to access it's menu.";
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2.0f - (ImGui::GetFontSize() * text.size() / 4.0f) + 25.0f, ImGui::GetWindowSize().y * 0.5f - (ImGui::GetFontSize() * text.size() / 16.0f)));
    ImGui::Text(text.c_str());
}

void GuiManager::DrawGuiSideShape() {}
void GuiManager::DrawGuiSideSpawner() {}
}