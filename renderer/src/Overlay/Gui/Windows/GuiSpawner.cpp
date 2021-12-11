#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

// TODO
void Renderer::ShowGuiSpawner(bool* p_open) {
    ImGui::SetNextWindowSize({400, 370}, ImGuiCond_Appearing);
    if (ImGui::Begin("Spawner", p_open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        ImGui::PushItemWidth((ImGui::GetWindowWidth() > G_WINDOW_LABEL_MIN) ? -G_WINDOW_LABEL_SPACE : G_WINDOW_LABEL_MIN - G_WINDOW_LABEL_SPACE - G_WINDOW_BORDER);

        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("Corpses")) {
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                const char* input_corpse_items[] = {"Circle", "Regular Polygon", "Oval Polygon"};
                const char* input_corpse_current_item = input_corpse_items[input_spawner.corpse_type];

                const char* input_spawn_items[] = {"Custom", "Explosion", "Conglomerate"};
                const char* input_spawn_current_item = input_spawn_items[input_spawner.spawn_type];

                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Spawner Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::BeginCombo("##combospawn", input_spawn_current_item)) {
                        for (int n = 0; n < IM_ARRAYSIZE(input_spawn_items); n++) {
                            bool is_selected = (input_spawn_current_item == input_spawn_items[n]);
                            if (ImGui::Selectable(input_spawn_items[n], is_selected)) {
                                input_spawn_current_item = input_spawn_items[n];
                                input_spawner.spawn_type = n;
                            }
                            if (is_selected) { ImGui::SetItemDefaultFocus(); }
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::SameLine();
                    ImGui::Text("Spawner");
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::BeginCombo("##combocorpse", input_corpse_current_item)) {
                        for (int n = 0; n < IM_ARRAYSIZE(input_corpse_items); n++) {
                            bool is_selected = (input_corpse_current_item == input_corpse_items[n]);
                            if (ImGui::Selectable(input_corpse_items[n], is_selected)) {
                                input_corpse_current_item = input_corpse_items[n];
                                input_spawner.corpse_type = n;
                            }
                            if (is_selected) { ImGui::SetItemDefaultFocus(); }
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::SameLine();
                    ImGui::Text("Corpse");
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    float input_spawner_position[2] = {input_spawner.positionX, input_spawner.positionY};
                    if (ImGui::DragFloat2("Position", input_spawner_position)) {
                        input_spawner.positionX = input_spawner_position[0];
                        input_spawner.positionY = input_spawner_position[1];
                    }
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragInt("Number", &input_spawner.corpse_number, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragInt("Duration", &input_spawner.duration, 0.1, -1, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragInt("Interval", &input_spawner.interval, 0.1, 0, +INT_MAX)) {}
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                if (ImGui::TreeNode("Corpse Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Checkbox("Fixed", &input_spawner.corpse_fixed);
                    ImGui::SameLine();
                    ImGui::Checkbox("Etherial", &input_spawner.corpse_etherial);
                    ImGui::SameLine();
                    ImGui::Checkbox("Tied", &input_spawner.corpse_tied);
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImVec4 temp_spawner_color =
                        ImVec4(input_spawner.corpse_color[0] / 255.0f, input_spawner.corpse_color[1] / 255.0f, input_spawner.corpse_color[2] / 255.0f, input_spawner.corpse_color[3] / 255.0f);
                    if (ImGui::ColorEdit4("Color", (float*)&temp_spawner_color)) {
                        input_spawner.corpse_color[0] = temp_spawner_color.x * 255.0f;
                        input_spawner.corpse_color[1] = temp_spawner_color.y * 255.0f;
                        input_spawner.corpse_color[2] = temp_spawner_color.z * 255.0f;
                        input_spawner.corpse_color[3] = temp_spawner_color.w * 255.0f;
                    }

                    if (input_spawner.corpse_type != 0) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragInt("Points num", &input_spawner.corpse_points_number, 0.1, 3, +INT_MAX)) {}
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("RadiusX", &input_spawner.corpse_radiusX, 0.1, 0, +INT_MAX)) {}

                    if (input_spawner.corpse_type == 2) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("RadiusY", &input_spawner.corpse_radiusY, 0.1, 0, +INT_MAX)) {}
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Rad randX", &input_spawner.corpse_radius_randomX, 0.1, 0, +INT_MAX)) {}

                    if (input_spawner.corpse_type == 2) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("Rad randY", &input_spawner.corpse_radius_randomY, 0.1, 0, +INT_MAX)) {}
                    }

                    if (input_spawner.spawn_type != 1) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("Pos randX", &input_spawner.corpse_position_randomX, 0.1, 0, +INT_MAX)) {}
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("Pos randY", &input_spawner.corpse_position_randomY, 0.1, 0, +INT_MAX)) {}
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Rotation", &input_spawner.corpse_rotation, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Rot rand", &input_spawner.corpse_rotation_random, 0.1, 0, +INT_MAX)) {}
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                if (ImGui::TreeNode("Launch Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Power", &input_spawner.launch_power, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Pow rand", &input_spawner.launch_random, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Direction", &input_spawner.launch_direction, 0.1, 0, 360)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Dir rand", &input_spawner.launch_direction_random, 0.1, 0, 360)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Rotation", &input_spawner.launch_rotation_power, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("Rot rand", &input_spawner.launch_rotation_random, 0.1, 0, +INT_MAX)) {}
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                if (ImGui::Button("Create Spawner", {-1, 23})) { this->spawners.push_back(this->input_spawner); }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::EndTabItem();
            }
            ImGui::Dummy(ImVec2(0.0f, 1.0f));
            if (ImGui::BeginTabItem("Particules")) { ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

}  // namespace ovl