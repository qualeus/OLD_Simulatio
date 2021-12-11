#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

void GuiManager::DrawGuiSideBar() {
    bool always_show = true;

    side_bar_size = ImGui::AnimationLinear(show_side_bar, side_bar_size, 0.0f, G_SIDE_BAR_SIZE, G_ANIMATION_SPEED);

    const float diff_size = G_SIDE_BAR_SIZE - side_bar_size;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 buttons_size = ImVec2(G_SIDE_BAR_SIZE, G_SIDE_BAR_SIZE * 0.8f);

    ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x - diff_size, viewport->GetWorkPos().y));
    ImGui::SetNextWindowSize(ImVec2(G_SIDE_BAR_SIZE, viewport->GetWorkSize().y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[F_ROBOTO_MEDIUM]);
    if (ImGui::Begin("SideBar", &always_show,
                     ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
        ImGuiStyle& style = ImGui::GetStyle();
        style.ItemSpacing = ImVec2(G_SIDE_BAR_SIZE / 10.0f, 0.0f);

        bool content_opened = show_side_content;
        if (content_opened) { ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]); }
        if (ImGui::Button(content_opened ? ICON_FK_LONG_ARROW_LEFT : ICON_FK_BARS, buttons_size)) { show_side_content = !show_side_content; }
        if (content_opened) { ImGui::PopStyleColor(); }

        bool shape_selected = input_side_menu == I_SIDE_MENU_SHAPE;
        if (shape_selected) { ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]); }
        if (ImGui::Button(ICON_FK_OBJECT_UNGROUP, buttons_size)) { input_side_menu = shape_selected ? I_SIDE_MENU_DEFAULT : I_SIDE_MENU_SHAPE; }
        if (shape_selected) { ImGui::PopStyleColor(); }

        bool spawner_selected = input_side_menu == I_SIDE_MENU_SPAWNER;
        if (spawner_selected) { ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]); }
        if (ImGui::Button(ICON_FK_ASTERISK, buttons_size)) { input_side_menu = spawner_selected ? I_SIDE_MENU_DEFAULT : I_SIDE_MENU_SPAWNER; }
        if (spawner_selected) { ImGui::PopStyleColor(); }

        // BOTTOM MENU
        ImGui::SetCursorPos(ImVec2(0.0f, viewport->GetWorkSize().y - buttons_size.y * 3));

        // MOUSE CURSOR
        std::vector<const char*> array_icons_input_mouse = {ICON_FK_MOUSE_POINTER, ICON_FK_ARROWS, ICON_FK_CROP};
        const bool mouse_selector_opened = ImGui::IsPopupOpen("##MouseSelectorWindow");
        if (mouse_selector_opened) { ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]); }
        if (ImGui::Button(array_icons_input_mouse[input_mouse_type], buttons_size)) { ImGui::OpenPopup("##MouseSelectorWindow"); }
        if (mouse_selector_opened) { ImGui::PopStyleColor(); }

        // ZOOM LEVEL
        const bool zoom_level_opened = ImGui::IsPopupOpen("##ZoomLevelWindow");
        if (zoom_level_opened) { ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]); }
        if (ImGui::Button(ICON_FK_SEARCH, buttons_size)) { ImGui::OpenPopup("##ZoomLevelWindow"); }
        if (zoom_level_opened) { ImGui::PopStyleColor(); }

        // SETTINGS
        if (ImGui::Button(ICON_FK_COG, buttons_size)) {}

        // BOTTOM MENU POPUPS
        ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);

        // MOUSE CURSOR
        ImGui::SetNextWindowSize(ImVec2(0.0f, buttons_size.y));
        ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x + buttons_size.x, viewport->GetWorkPos().y + viewport->GetWorkSize().y - buttons_size.y * 3));
        if (ImGui::BeginPopup("##MouseSelectorWindow", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_PopupBg]);
            for (int i = 0; i < array_icons_input_mouse.size(); i++) {
                ImGui::SameLine(0.0f, 0.0f);
                const bool selected_button = i == input_mouse_type;
                if (selected_button) { ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]); }
                if (ImGui::Button(array_icons_input_mouse[i], buttons_size)) {
                    input_mouse_type = i;
                    ImGui::CloseCurrentPopup();
                }
                if (selected_button) { ImGui::PopStyleColor(); }
            }
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }

        // ZOOM LEVEL
        ImGui::SetNextWindowSize(ImVec2(0.0f, buttons_size.y));
        ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x + buttons_size.x, viewport->GetWorkPos().y + viewport->GetWorkSize().y - buttons_size.y * 2));
        if (ImGui::BeginPopup("##ZoomLevelWindow", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_PopupBg]);
            if (ImGui::Button(ICON_FK_SEARCH_MINUS, buttons_size)) {
                if (get_camera_size() > 0.1f) { set_camera_size(get_camera_size() + 0.1f); }
            }
            ImGui::SameLine(0.0f, 0.0f);
            float temp_camera_zoom = 100.0f * (100.0f / get_camera_zoom());
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
            if (ImGui::SliderFloat("##SliderFloatZoom", &temp_camera_zoom, 0.1f, 1000.0f, "%.1f%%")) { set_camera_zoom(100.0f * (100.0f / temp_camera_zoom)); }
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);
            if (ImGui::Button(ICON_FK_SEARCH_PLUS, buttons_size)) {
                if (get_camera_size() < 1000.0f) { set_camera_size(get_camera_size() - 0.1f); }
            }
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
        ImGui::End();
    }
    ImGui::PopFont();

    const float center_handle = buttons_size.y / 2.0f;
    ImVec2 handle_size = ImVec2(G_HANDLE_SIZE, G_SIDE_BAR_SIZE);

    ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x - diff_size + buttons_size.x, viewport->GetCenter().y - center_handle));
    ImGui::SetNextWindowSize(handle_size);

    if (side_content_size == 0.0f) {
        if (ImGui::Begin("##SideWindowHandle", &always_show,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_NoDocking)) {
            if (ImGui::Button(show_side_bar ? ICON_FK_CARET_LEFT : ICON_FK_CARET_RIGHT, handle_size)) { show_side_bar = !show_side_bar; }
            ImGui::End();
        }
    }

    ImGui::PopStyleVar(3);
}
}