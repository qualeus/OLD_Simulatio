#include "../../../../include/Overlay/Gui/GuiManager.hpp"

namespace ovl {

void GuiManager::DrawGuiMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                std::string path =
                    srz::FileManager::SelectFile("Open System", srz::FileManager::CurrentPath(), "System (.json, .latio)\0*.json;*.latio\0System (.json)\0*.json\0System (.latio)\0*.latio\0", false);
                if (path != "") {
                    phy::System temp = srz::FileManager::LoadSystem(path);
                    *this->system = temp;
                }
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                std::string path =
                    srz::FileManager::SelectFile("Save System", srz::FileManager::CurrentPath(), "System (.json, .latio)\0*.json;*.latio\0System (.json)\0*.json\0System (.latio)\0*.latio\0", true);
                phy::System temp = phy::System(*this->system);
                if (path != "") { srz::FileManager::SaveSystem(temp, path); }
            }

            if (ImGui::MenuItem("Clear", "Ctrl+R")) { show_gui["popup_clear"] = true; }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Settings")) { show_gui["settings"] = true; }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Exit")) {
                // Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do stuff */
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Do stuff */
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::BeginMenu("Layout")) {
                if (ImGui::MenuItem("Reset Base Layout")) { reset_base_layout = true; }
                ImGui::EndMenu();
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Console", NULL, &show_gui["console"]);
            ImGui::MenuItem("Properties", NULL, &show_gui["properties"]);
            ImGui::MenuItem("Spawner", NULL, &show_gui["spawner"]);
            ImGui::MenuItem("Debug Overlay", NULL, &show_gui["overlay"]);
            ImGui::MenuItem("ImGui Demo", NULL, &show_gui["imguidemo"]);
            ImGui::MenuItem("Benchmark", NULL, &show_gui["benchmark"]);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug")) {
            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Quadtree", NULL, &show_debug["quadtree"]);
            ImGui::MenuItem("Bounds", NULL, &show_debug["bounds"]);
            ImGui::MenuItem("Centroids", NULL, &show_debug["centroids"]);
            ImGui::MenuItem("Edges", NULL, &show_debug["edges"]);
            ImGui::MenuItem("Projections", NULL, &show_debug["projections"]);
            ImGui::MenuItem("Vertices", NULL, &show_debug["vertices"]);
            ImGui::MenuItem("Normals", NULL, &show_debug["normals"]);
            ImGui::MenuItem("Velocity", NULL, &show_debug["velocity"]);
            ImGui::MenuItem("XYVelocity", NULL, &show_debug["xyvelocity"]);
            ImGui::MenuItem("Pairs", NULL, &show_debug["pairs"]);
            ImGui::MenuItem("QuadPairs", NULL, &show_debug["quadpairs"]);
            ImGui::MenuItem("Contacts", NULL, &show_debug["contacts"]);
            ImGui::MenuItem("Collisions", NULL, &show_debug["collisions"]);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Shaders")) {
            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Grid", NULL, &post_process["grid"]);
            ImGui::MenuItem("Gravity", NULL, &post_process["gravity"]);
            ImGui::MenuItem("Blur", NULL, &post_process["blur"]);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Documentation")) { /* Do stuff */
            }
            if (ImGui::MenuItem("About")) { /* Do stuff */
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
}  // namespace ovl