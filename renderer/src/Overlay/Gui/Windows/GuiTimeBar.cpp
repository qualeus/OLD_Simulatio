
#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

void GuiManager::DrawGuiTimeBar() {
    bool always_show = true;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    const float center_windows = (G_SIDE_BAR_SIZE * 5.0f) / 2.0f;
    ImVec2 buttons_size = ImVec2(G_SIDE_BAR_SIZE, G_TIME_BAR_SIZE);

    time_bar_size = ImGui::AnimationLinear(show_time_bar, time_bar_size, 0.0f, G_TIME_BAR_SIZE, G_ANIMATION_SPEED);

    const float diff_size = G_TIME_BAR_SIZE - time_bar_size;

    ImGui::SetNextWindowPos(ImVec2(viewport->GetCenter().x - center_windows, viewport->GetWorkPos().y - diff_size));
    ImGui::SetNextWindowSize(ImVec2(0.0f, buttons_size.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    if (ImGui::Begin(
            "##TimeWindow", &always_show,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking)) {
        ImGui::Button(ICON_FK_FAST_BACKWARD, buttons_size);
        if (ImGui::IsItemActive()) {
            this->debug_system_edited = true;
            this->system.set_dt(this->system.get_dt() - dt_step);
        }
        ImGui::SameLine(0.0f, 0.0f);
        if (ImGui::Button(ICON_FK_BACKWARD, buttons_size)) {
            this->system.invert_dt();
            this->system.Step();
            this->system.invert_dt();
            this->UpdateCorpseDatas();
            this->UpdateSpawners();
        }
        ImGui::SameLine(0.0f, 0.0f);
        if (ImGui::Button(paused ? ICON_FK_PLAY : ICON_FK_PAUSE, buttons_size)) {
            this->debug_system_edited = true;
            Pause();
        }
        ImGui::SameLine(0.0f, 0.0f);
        if (ImGui::Button(ICON_FK_FORWARD, buttons_size)) {
            this->system.Step();
            this->UpdateCorpseDatas();
            this->UpdateSpawners();
        }
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Button(ICON_FK_FAST_FORWARD, buttons_size);
        if (ImGui::IsItemActive()) {
            this->debug_system_edited = true;
            this->system.set_dt(this->system.get_dt() + dt_step);
        }
        ImGui::End();
    }

    const float center_handle = G_SIDE_BAR_SIZE / 2.0f;
    ImVec2 handle_size = ImVec2(G_SIDE_BAR_SIZE, G_HANDLE_SIZE);

    ImGui::SetNextWindowPos(ImVec2(viewport->GetCenter().x - center_handle, viewport->GetWorkPos().y + buttons_size.y - diff_size));
    ImGui::SetNextWindowSize(handle_size);

    if (ImGui::Begin(
            "##TimeWindowHandle", &always_show,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking)) {
        if (ImGui::Button(show_time_bar ? ICON_FK_CARET_UP : ICON_FK_CARET_DOWN, handle_size)) { show_time_bar = !show_time_bar; }
        ImGui::End();
    }
    ImGui::PopStyleVar(3);
}
}