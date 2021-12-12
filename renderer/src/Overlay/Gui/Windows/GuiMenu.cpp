#include "../../../../include/Overlay/Gui/GuiManager.hpp"

namespace ovl {

void GuiManager::DrawGuiMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                std::string path = srz::FileManager::SelectFile("Open System", srz::FileManager::CurrentPath(), "System (.latio)\0*.latio\0", false);
                if (path != "") {
                    phy::System temp = srz::FileManager::LoadSystem(path);
                    this->system->Assign(temp);
                    std::cout << "gravity: " << this->system->get_gravity() << std::endl;
                }
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                std::string path = srz::FileManager::SelectFile("Save System", srz::FileManager::CurrentPath(), "System (.latio)\0*.latio\0", true);
                if (path != "") { srz::FileManager::SaveSystem(system, path); }
            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) {}

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Settings")) { show_gui_settings = true; }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Exit")) {
                // Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear", "Ctrl+R")) { show_popup_clear_system = true; }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

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
            ImGui::MenuItem("Console", NULL, &show_gui_console);
            ImGui::MenuItem("Properties", NULL, &show_gui_properties);
            ImGui::MenuItem("Spawner", NULL, &show_gui_spawner);
            ImGui::MenuItem("Debug Overlay", NULL, &show_gui_overlay);
            ImGui::MenuItem("ImGui Demo", NULL, &show_gui_imguidemo);
            ImGui::MenuItem("Benchmark", NULL, &show_gui_benchmark);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug")) {
            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Quadtree", NULL, &debug_show_quadtree);
            ImGui::MenuItem("Bounds", NULL, &debug_show_bounds);
            ImGui::MenuItem("Centroids", NULL, &debug_show_centroids);
            ImGui::MenuItem("Edges", NULL, &debug_show_edges);
            ImGui::MenuItem("Projections", NULL, &debug_show_projections);
            ImGui::MenuItem("Vertices", NULL, &debug_show_vertices);
            ImGui::MenuItem("Normals", NULL, &debug_show_normals);
            ImGui::MenuItem("Velocity", NULL, &debug_show_velocity);
            ImGui::MenuItem("XYVelocity", NULL, &debug_show_xyvelocity);
            ImGui::MenuItem("Pairs", NULL, &debug_show_pairs);
            ImGui::MenuItem("QuadPairs", NULL, &debug_show_quadpairs);
            ImGui::MenuItem("Contacts", NULL, &debug_show_contacts);
            ImGui::MenuItem("Collisions", NULL, &debug_show_collisions);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Shaders")) {
            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Grid", NULL, &post_process_grid);
            ImGui::MenuItem("Gravity", NULL, &post_process_gravity);
            ImGui::MenuItem("Blur", NULL, &post_process_blur);
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