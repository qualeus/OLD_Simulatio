#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

// TODO
void GuiManager::ShowGuiSettingsInterface() {
    ImGui::Text("Interface Style");

    ImGui::Dummy(ImVec2(0.0f, 7.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    int style_idx = 0;
    if (ImGui::Combo("Theme", &style_idx, "Dark\0Classic\0Light\0")) {
        switch (style_idx) {
            case 0: ImGui::StyleColorsDark(); break;
            case 1: ImGui::StyleColorsClassic(); break;
            case 2: ImGui::StyleColorsLight(); break;
        }
    }
}

void Renderer::ShowGuiSettings(bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    if (ImGui::Begin("Settings", p_open)) {
        ImGui::BeginChild("SettingsMenu", ImVec2(200, 0), true);
        ImGui::Text("Settings");

        ImGui::Dummy(ImVec2(0.0f, 7.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        static int selected_menu = 0;
        ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth;
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("WorkBench")) {
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
            if (ImGui::TreeNodeEx("Styles", ImGuiTreeNodeFlags_SpanFullWidth)) {
                node_flags = base_flags;
                if (selected_menu == S_MENU_INTERFACE) node_flags |= ImGuiTreeNodeFlags_Selected;

                ImGui::TreeNodeEx("Interface", node_flags, "Interface", S_MENU_INTERFACE);
                if (ImGui::IsItemClicked()) selected_menu = S_MENU_INTERFACE;

                node_flags = base_flags;
                if (selected_menu == S_MENU_SIMULATION) node_flags |= ImGuiTreeNodeFlags_Selected;

                ImGui::TreeNodeEx("Simulation", node_flags, "Simulation", S_MENU_SIMULATION);
                if (ImGui::IsItemClicked()) selected_menu = S_MENU_SIMULATION;

                node_flags = base_flags;
                if (selected_menu == S_MENU_CONSOLE) node_flags |= ImGuiTreeNodeFlags_Selected;

                ImGui::TreeNodeEx("Console", node_flags, "Console", S_MENU_CONSOLE);
                if (ImGui::IsItemClicked()) selected_menu = S_MENU_CONSOLE;
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Window")) {
            if (ImGui::TreeNodeEx("...", ImGuiTreeNodeFlags_SpanFullWidth)) { ImGui::TreePop(); }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Simulation")) {
            if (ImGui::TreeNodeEx("...", ImGuiTreeNodeFlags_SpanFullWidth)) { ImGui::TreePop(); }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Preferences")) {
            if (ImGui::TreeNodeEx("Shortcuts", ImGuiTreeNodeFlags_SpanFullWidth)) { ImGui::TreePop(); }
            ImGui::TreePop();
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("SettingsPage", ImVec2(0, 0), true);

        switch (selected_menu) {
            case S_MENU_INTERFACE: {
                ShowGuiSettingsInterface();
            } break;
            case S_MENU_SIMULATION: {
                ImGui::ShowStyleEditor();
            } break;
            case S_MENU_CONSOLE: {
                ImGui::ShowStyleSelector("style");
            } break;

            default: {
            } break;
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::End();
    }
}

}