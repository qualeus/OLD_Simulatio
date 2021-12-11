
#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {
// TODO
void Renderer::ShowGuiProperties(bool* p_open) {
    if (ImGui::Begin("Properties", p_open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        char system_name[32];
        char selection_name[32];
        ImFormatString(system_name, IM_ARRAYSIZE(system_name), "System");  //"System <%s>", name.c_str());
        ImFormatString(selection_name, IM_ARRAYSIZE(selection_name), "Selection");

        ImGui::PushItemWidth((ImGui::GetWindowWidth() > G_WINDOW_LABEL_MIN) ? -G_WINDOW_LABEL_SPACE : G_WINDOW_LABEL_MIN - G_WINDOW_LABEL_SPACE - G_WINDOW_BORDER);

        if (ImGui::BeginTabBar("MyTabBar")) {
            /* SYSTEM PROPERTIES */
            if (ImGui::BeginTabItem(system_name)) {
                ImGui::Dummy(ImVec2(0.0f, 1.2f));

                /* PRECISION */
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ("SIMULATIO_PRECISION:" + gmt::to_string(gmt::type_name<SIMULATIO_PRECISION>())).c_str());
                ImGui::Dummy(ImVec2(0.0f, 7.0f));

                ImGui::SetNextTreeNodeOpen(false, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("System Infos")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    /* System Name */
                    char temp_name[32];
                    ImFormatString(temp_name, IM_ARRAYSIZE(temp_name), name.c_str());
                    if (ImGui::InputText("Name", temp_name, IM_ARRAYSIZE(temp_name), ImGuiInputTextFlags_EnterReturnsTrue)) { name = temp_name; }
                    // ImGui::SetKeyboardFocusHere(-1);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Text("Units");
                    ImGui::SameLine();
                    ImGui::Help("");
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* Lenght Unit */
                    char temp_lenght_unit[32];
                    ImFormatString(temp_lenght_unit, IM_ARRAYSIZE(temp_lenght_unit), lenght_unit.c_str());
                    if (ImGui::InputText("Lenght", temp_lenght_unit, IM_ARRAYSIZE(temp_lenght_unit), ImGuiInputTextFlags_EnterReturnsTrue)) { lenght_unit = temp_lenght_unit; }

                    /* Mass Unit */
                    char temp_mass_unit[32];
                    ImFormatString(temp_mass_unit, IM_ARRAYSIZE(temp_mass_unit), mass_unit.c_str());
                    if (ImGui::InputText("Mass", temp_mass_unit, IM_ARRAYSIZE(temp_mass_unit), ImGuiInputTextFlags_EnterReturnsTrue)) { mass_unit = temp_mass_unit; }

                    /* Time Unit */
                    char temp_time_unit[32];
                    ImFormatString(temp_time_unit, IM_ARRAYSIZE(temp_time_unit), time_unit.c_str());
                    if (ImGui::InputText("Time", temp_time_unit, IM_ARRAYSIZE(temp_time_unit), ImGuiInputTextFlags_EnterReturnsTrue)) { time_unit = temp_time_unit; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("System Settings")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::Text("Reference Frame gravity field");
                    ImGui::SameLine();
                    ImGui::Help(
                        "The calculated forces do not take into\n"
                        "account the mass of the objects on which\n"
                        "they are applied (as on the Earth).\n\n"
                        "We place ourselves here in a frame of\n"
                        "reference where a much more massive\n"
                        "object is the origin of an homogeneous\n"
                        "gravity field.");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Force_X */
                    float temp_force_x = system.get_force_x();
                    if (ImGui::DragFloat("Force X", &temp_force_x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) { system.set_force_x(temp_force_x); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    /* System Force_Y */
                    float temp_force_y = system.get_force_y();
                    if (ImGui::DragFloat("Force Y", &temp_force_y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) { system.set_force_y(temp_force_y); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Delta Time */
                    float temp_dt = system.get_dt();
                    if (ImGui::InputFloat("dt", &temp_dt, 0.001f, 0.1f, "%.3f")) { system.set_dt(temp_dt); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Limits */
                    gmt::BoundsI rect_limits = system.get_limits();
                    float temp_limits[4] = {static_cast<float>(rect_limits.x1), static_cast<float>(rect_limits.y1), static_cast<float>(rect_limits.x2), static_cast<float>(rect_limits.y2)};

                    if (!system.get_enable_limits()) {
                        ImGuiStyle& style = ImGui::GetStyle();
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_TableHeaderBg]);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.Colors[ImGuiCol_TextDisabled]);
                    }

                    if (ImGui::DragFloat4("##label", temp_limits, 20.f, -FLT_MAX, +FLT_MAX, "%.f")) {
                        rect_limits = gmt::BoundsI(temp_limits[0], temp_limits[1], temp_limits[2], temp_limits[3]);
                        system.set_limits(rect_limits);
                    }

                    if (!system.get_enable_limits()) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleColor(2);
                    }

                    ImGui::SameLine();
                    bool temp_enable_limits = system.get_enable_limits();
                    if (ImGui::Checkbox("Limits", &temp_enable_limits)) { system.set_enable_limits(temp_enable_limits); }
                    ImGui::SameLine();
                    ImGui::Help("Pos (x,y) => center of the rectangle\nSize (x,y) => size of the rectangle");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* System Enable Gravity */
                    bool temp_gravity = system.get_gravity();
                    if (ImGui::Checkbox("Enable Gravity", &temp_gravity)) { system.set_gravity(temp_gravity); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Light Speed */
                    float temp_LS = system.get_LS();
                    if (ImGui::InputFloat("LS", &temp_LS, temp_LS / 1000.0f, temp_LS / 10.0f, "%e")) { system.set_LS(temp_LS); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }
                    ImGui::SameLine();
                    ImGui::Help(
                        "The speed of light in vacuum, commonly denoted c.\n"
                        "Its exact value is defined as 299 792 458 m.s-1\n"
                        "(approximately 300 000 km.s-1)");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Gravity */
                    float temp_G = system.get_G();
                    if (ImGui::InputFloat("G", &temp_G, temp_G / 1000.0f, temp_G / 10.0f, "%e")) { system.set_G(temp_G); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }
                    ImGui::SameLine();
                    ImGui::Help(
                        "The gravitational constant (also known as the\n"
                        "Newtonian constant of gravitation). Its value\n"
                        "is approximately 6.674×10-11 m3.kg-1.s-2");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    ImGui::Checkbox("Lock Accuracy", &locked_accuracy);
                    ImGui::SameLine();
                    ImGui::Help("Warning: Accuracy greatly impact the performances");

                    if (locked_accuracy) {
                        ImGuiStyle& style = ImGui::GetStyle();
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_TableHeaderBg]);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.Colors[ImGuiCol_TextDisabled]);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Collisions Accuracy */
                    int temp_collision_accuracy = system.get_collision_accuracy();
                    if (ImGui::SliderInt("Collisions", &temp_collision_accuracy, 1, 100, "x%d")) { system.set_collision_accuracy(temp_collision_accuracy); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Constraints Accuracy */
                    int temp_constraint_accuracy = system.get_constraint_accuracy();
                    if (ImGui::SliderInt("Constraints", &temp_constraint_accuracy, 1, 100, "x%d")) { system.set_constraint_accuracy(temp_constraint_accuracy); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    if (locked_accuracy) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleColor(2);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Inputs Settings")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::Checkbox("Enable Inputs", &enable_inputs);
                    ImGui::SameLine();
                    if (ImGui::Checkbox("Paused", &paused)) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Camera Position */
                    float temp_camera_pos[2] = {get_camera_x(), get_camera_y()};
                    if (ImGui::DragFloat2("Camera", temp_camera_pos, 1.f, -FLT_MAX, +FLT_MAX, "%.f")) {
                        set_camera_x(temp_camera_pos[0]);
                        set_camera_y(temp_camera_pos[1]);
                    }
                    ImGui::SameLine();
                    ImGui::Help("Camera Position (x,y) => center of the screen");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* Renderer Camera Zoom */
                    float temp_camera_zoom = 100.0f * (100.0f / get_camera_zoom());
                    if (ImGui::DragFloat("Zoom", &temp_camera_zoom, 0.5f, -FLT_MAX, +FLT_MAX, "%.1f%%")) { set_camera_zoom(100.0f * (100.0f / temp_camera_zoom)); }
                    ImGui::SameLine();
                    ImGui::Help("Camera Zoom =>  100.0f * (100.0f / Scale)");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* Renderer Window Size */
                    int temp_screen_size[2] = {get_screen_width(), get_screen_height()};
                    if (ImGui::DragInt2("Window", temp_screen_size, 1.f, 0, +INT_MAX, "%dpx")) {
                        set_screen_width(temp_screen_size[0]);
                        set_screen_height(temp_screen_size[1]);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::SliderFloat("Lauch Power", &launch_power, -5, 5, "%.1f");
                    ImGui::SliderFloat("Zoom Speed", &zoom_speed, -1, 1, "%.2f");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Graphical Settings")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* Renderer Background Color */
                    ImVec4 temp_background_color = ImVec4(background_color.r / 255.0f, background_color.g / 255.0f, background_color.b / 255.0f, background_color.a / 255.0f);
                    if (ImGui::ColorEdit4("Background", (float*)&temp_background_color)) {
                        background_color = sf::Color(temp_background_color.x * 255.0f, temp_background_color.y * 255.0f, temp_background_color.z * 255.0f, temp_background_color.w * 255.0f);
                    }
                    ImGui::SameLine();
                    ImGui::Help("Right click on the color picker\nto change its format, or even\nexport the color.");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* Renderer Framerate */
                    int temp_max_framerate = get_max_framerate();
                    const char* items[] = {"30 Hz", "60 Hz", "120 Hz", "No Limit"};
                    int item_current = 0;

                    if (ImGui::Combo("Framerate", &item_current, items, IM_ARRAYSIZE(items))) {
                        switch (item_current) {
                            case 0: set_max_framerate(30); break;
                            case 1: set_max_framerate(60); break;
                            case 2: set_max_framerate(120); break;
                            default: set_max_framerate(INT_MAX);
                        }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Line Thickness */
                    ImGui::InputInt("Line Thickness", &line_thickness);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Outline Thickness */
                    ImGui::InputInt("Outline Thickness", &outline_thickness);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Text Resolution */
                    float temp_text_resolution = text_resolution;
                    if (ImGui::InputFloat("Text", &temp_text_resolution, 0.1f, 1.0f, "%.1fpx")) {
                        if (temp_text_resolution > 0) { text_resolution = temp_text_resolution; }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Vector Size */
                    float temp_vector_size = vector_size;
                    if (ImGui::InputFloat("Vector", &temp_vector_size, 0.5f, 1.0f, "%.1fpx")) {
                        if (temp_vector_size > 0) { vector_size = temp_vector_size; }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Velocity Size */
                    float temp_velocity_size = velocity_size;
                    if (ImGui::InputFloat("Velocity", &temp_velocity_size, 0.1f, 1.0f, "%.1funits")) {
                        if (temp_velocity_size > 0) { velocity_size = temp_velocity_size; }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Arrow Size */
                    int temp_arrow_size = arrow_size;
                    if (ImGui::InputInt("Arrow", &temp_arrow_size)) {
                        if (temp_arrow_size > 0) { arrow_size = temp_arrow_size; }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    ImGui::TreePop();
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(selection_name)) {
                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                /* Type de selection */
                char selection_label[30];
                if (selected_corpses_cursor.size() == 0) {
                    ImFormatString(selection_label, IM_ARRAYSIZE(selection_label), "Selection <%s>", "empty");
                } else if (selected_corpses_cursor.size() == 1) {
                    ImFormatString(selection_label, IM_ARRAYSIZE(selection_label), "Selection <1 body [ID %i] >", system.get_corpse(selected_corpses_cursor[0])->get_id());
                } else {
                    ImFormatString(selection_label, IM_ARRAYSIZE(selection_label), "Selection <%i bodies>", selected_corpses_cursor.size());
                }
                ImGui::Text(selection_label);
                int selected_index = 0;

                /* Selection Multiple */
                if (selected_corpses_cursor.size() > 1) {
                    ImGui::SameLine();

                    char chr_default[30];
                    ImFormatString(chr_default, IM_ARRAYSIZE(chr_default), "<%i bodies>", selected_corpses_cursor.size());
                    const char* current_item = chr_default;
                    if (selected_index > 0) { current_item = std::to_string(selected_corpses_cursor[selected_index - 1]).c_str(); }
                    ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

                    ImGuiStyle& style = ImGui::GetStyle();
                    float w = ImGui::CalcItemWidth();
                    float spacing = style.ItemInnerSpacing.x;
                    float button_sz = ImGui::GetFrameHeight();
                    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);

                    if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton)) {
                        bool default_selected = (0 == selected_index);
                        if (ImGui::Selectable(chr_default, default_selected)) { selected_index = 0; }
                        if (default_selected) { ImGui::SetItemDefaultFocus(); }

                        for (int i = 1; i < selected_corpses_cursor.size() + 1; i++) {
                            const char* chr_index = std::to_string(selected_corpses_cursor[i - 1]).c_str();

                            bool is_selected = (i == selected_index);
                            if (ImGui::Selectable(chr_index, is_selected)) { selected_index = i; }
                            if (is_selected) { ImGui::SetItemDefaultFocus(); }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::PopItemWidth();
                    ImGui::SameLine(0, spacing);
                    if (ImGui::ArrowButton("##l", ImGuiDir_Left)) {
                        if (selected_index > 0) { selected_index--; }
                    }
                    ImGui::SameLine(0, spacing);
                    if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
                        if (selected_index < selected_corpses_cursor.size()) { selected_index++; }
                    }
                }

                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0.0f, 7.0f));

                bool nothing_selected = (selected_corpses_cursor.size() == 0);
                bool unique_selected = (selected_corpses_cursor.size() == 1);
                int cursor_selected = -1;

                if (unique_selected) { selected_index = 0; }
                if (nothing_selected) { selected_index = 0; }

                if (!nothing_selected) {
                    if (unique_selected) {
                        cursor_selected = selected_corpses_cursor[0];
                        trajectory_debug_index = selected_corpses_cursor[0];
                    } else {
                        if (selected_index > 0) {
                            cursor_selected = selected_corpses_cursor[selected_index - 1];
                            trajectory_debug_index = selected_corpses_cursor[selected_index - 1];
                            unique_selected = true;
                        }
                    }
                }

                ImGui::Dummy(ImVec2(0.0f, 1.2f));
                ImGui::SetNextTreeNodeOpen(false, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Corpse Infos")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (nothing_selected) {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::TextDisabled("Select one or more objects to see it's infos");
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    } else {
                        float props_vel;
                        float props_acc;
                        float props_jer;

                        float props_ek;
                        float props_ep;
                        float props_em;

                        char label_props[30] = "Body Properties";

                        int body_cursor;
                        if (unique_selected) {
                            body_cursor = cursor_selected;
                        } else {
                            body_cursor = selected_corpses_cursor[0];
                        }
                        props_vel = system.get_corpse(body_cursor)->get_diff_pos().Magnitude();  // dp / dt
                        props_acc = 10.0f;
                        props_jer = 10.0f;

                        props_ek = 0.5f * system.get_corpse(body_cursor)->get_mass() * props_vel * props_vel;  // Ek = (1/2)mv^2
                        props_ep = 10.0f;
                        props_em = system.get_corpse(body_cursor)->get_mass() * system.get_LS() * system.get_LS();  // Em = m.c^2

                        if (!unique_selected) { ImFormatString(label_props, IM_ARRAYSIZE(label_props), "Body Properties <Mean>"); }
                        ImGui::Text(label_props);
                        ImGui::SameLine();
                        ImGui::Help("Kinetic Energy = (1/2).m.v^2\nPotential Energy = \nEnergy = m.c^2");
                        ImGui::Text(
                            " \n"
                            " * Velocity: %.f (m.s^-1)\n"
                            " * Acceleration: %.f (m.s^-2)\n"
                            " * Jerk: %.f (m.s^-3)\n"
                            "\n"
                            " * Ek = %.f (J)\n"
                            " * Ep = %.f (J)\n"
                            " * Em = %.f (J)\n"
                            "\n",
                            props_vel, props_acc, props_jer, props_ek, props_ep, props_em);
                    }
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }

                ImGui::Dummy(ImVec2(0.0f, 7.0f));

                /* CORPSE PROPERTIES */
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Corpse Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    if (nothing_selected) {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::TextDisabled("Select one or more objects to edit it's properties");
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    } else {
                        float temp_position_x;
                        float temp_position_y;
                        float temp_velocity_x;
                        float temp_velocity_y;

                        float temp_friction;
                        float temp_mass;
                        float temp_damping;

                        int temp_fixed;
                        int temp_etherial;
                        int temp_tied;

                        float temp_rotation;
                        float temp_spin;
                        float temp_motor;

                        char label_posX[30] = "Position X";
                        char label_posY[30] = "Position Y";
                        char label_velX[30] = "Velocity X";
                        char label_velY[30] = "Velocity Y";

                        char label_fric[30] = "Friction";
                        char label_mass[30] = "Mass";
                        char label_damp[30] = "Damping";

                        char label_fix[30] = "Fixed";
                        char label_eth[30] = "Etherial";
                        char label_tie[30] = "Tied";

                        char label_rot[30] = "Rotation";
                        char label_spi[30] = "Spin";
                        char label_mot[30] = "Motor";

                        int body_cursor;

                        if (unique_selected) {
                            body_cursor = cursor_selected;
                        } else {
                            body_cursor = selected_corpses_cursor[0];
                        }

                        temp_position_x = system.get_corpse(body_cursor)->get_pos_x();
                        temp_position_y = system.get_corpse(body_cursor)->get_pos_y();
                        temp_velocity_x = system.get_corpse(body_cursor)->get_diff_pos_x();
                        temp_velocity_y = system.get_corpse(body_cursor)->get_diff_pos_y();

                        temp_friction = system.get_corpse(body_cursor)->get_friction();
                        temp_mass = system.get_corpse(body_cursor)->get_mass();
                        temp_damping = system.get_corpse(body_cursor)->get_damping();

                        temp_fixed = system.get_corpse(body_cursor)->get_fixed();
                        temp_etherial = system.get_corpse(body_cursor)->get_etherial();
                        temp_tied = system.get_corpse(body_cursor)->get_tied();

                        temp_rotation = system.get_corpse(body_cursor)->get_rotation();
                        temp_spin = system.get_corpse(body_cursor)->get_diff_rotation();
                        temp_motor = system.get_corpse(body_cursor)->get_motor();

                        if (!unique_selected) {
                            for (int i = 1; i < selected_corpses_cursor.size(); i++) {
                                int temp_cursor = selected_corpses_cursor[i];
                                float sig_pos = 0.1f;
                                float sig_vel = 0.01f;
                                float sig_com = 0.001f;

                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_pos_x(), temp_position_x, sig_pos)) {
                                    ImFormatString(label_posX, IM_ARRAYSIZE(label_posX), "Position X <differ>");
                                    temp_position_x = 0.0f;
                                }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_pos_y(), temp_position_y, sig_pos)) {
                                    ImFormatString(label_posY, IM_ARRAYSIZE(label_posY), "Position Y <differ>");
                                    temp_position_y = 0.0f;
                                }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_diff_pos_x(), temp_velocity_x, sig_vel)) {
                                    ImFormatString(label_velX, IM_ARRAYSIZE(label_velX), "Velocity X <differ>");
                                    temp_velocity_x = 0.0f;
                                }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_diff_pos_y(), temp_velocity_y, sig_vel)) {
                                    ImFormatString(label_velY, IM_ARRAYSIZE(label_velY), "Velocity Y <differ>");
                                    temp_velocity_y = 0.0f;
                                }

                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_friction(), temp_friction, sig_com)) {
                                    ImFormatString(label_fric, IM_ARRAYSIZE(label_fric), "Fricition <differ>");
                                }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_mass(), temp_mass, sig_com)) { ImFormatString(label_mass, IM_ARRAYSIZE(label_mass), "Mass <differ>"); }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_damping(), temp_damping, sig_com)) {
                                    ImFormatString(label_damp, IM_ARRAYSIZE(label_damp), "Damping <differ>");
                                }

                                if (!system.get_corpse(temp_cursor)->get_fixed() == temp_fixed) {
                                    ImFormatString(label_fix, IM_ARRAYSIZE(label_fix), "Fixed <differ>");
                                    temp_fixed = -1;
                                }
                                if (!system.get_corpse(temp_cursor)->get_etherial() == temp_etherial) {
                                    ImFormatString(label_eth, IM_ARRAYSIZE(label_eth), "Etherial <differ>");
                                    temp_etherial = -1;
                                }
                                if (!system.get_corpse(temp_cursor)->get_tied() == temp_tied) {
                                    ImFormatString(label_tie, IM_ARRAYSIZE(label_tie), "Tied <differ>");
                                    temp_tied = -1;
                                }

                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_rotation(), temp_rotation, sig_pos)) {
                                    ImFormatString(label_rot, IM_ARRAYSIZE(label_rot), "Rotation <differ>");
                                    temp_rotation = 0.0f;
                                }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_rotation(), temp_spin, sig_pos)) {
                                    ImFormatString(label_spi, IM_ARRAYSIZE(label_spi), "Spin <differ>");
                                    temp_spin = 0.0f;
                                }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_motor(), temp_motor, sig_pos)) {
                                    ImFormatString(label_mot, IM_ARRAYSIZE(label_mot), "Motor <differ>");
                                    temp_motor = 0.0f;
                                }
                            }
                        }

                        ImGui::Dummy(ImVec2(0.0f, 5.0f));
                        ImGui::Separator();
                        ImGui::Dummy(ImVec2(0.0f, 5.0f));

                        /* Corpse Position_X */
                        if (ImGui::DragFloat(label_posX, &temp_position_x, 0.5f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->Move(gmt::VectorI(temp_position_x, system.get_corpse(cursor_selected)->get_pos_y()));
                                system.get_corpse(cursor_selected)->Stop();
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->Move(gmt::VectorI(system.get_corpse(body_cursor)->get_pos_x() + temp_position_x, system.get_corpse(body_cursor)->get_pos_y()));
                                    system.get_corpse(body_cursor)->Stop();
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        /* Corpse Position_Y */
                        if (ImGui::DragFloat(label_posY, &temp_position_y, 0.5f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->Move(gmt::VectorI(system.get_corpse(cursor_selected)->get_pos_x(), temp_position_y));
                                system.get_corpse(cursor_selected)->Stop();
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->Move(gmt::VectorI(system.get_corpse(body_cursor)->get_pos_x(), system.get_corpse(body_cursor)->get_pos_y() + temp_position_y));
                                    system.get_corpse(body_cursor)->Stop();
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::Dummy(ImVec2(0.0f, 5.0f));

                        /* Corpse Velocity_X */
                        if (ImGui::DragFloat(label_velX, &temp_velocity_x, 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_last_pos_x(system.get_corpse(cursor_selected)->get_pos_x() - temp_velocity_x);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_last_pos_x(system.get_corpse(body_cursor)->get_last_pos_x() - temp_velocity_x);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        /* Corpse Velocity_Y */
                        if (ImGui::DragFloat(label_velY, &temp_velocity_y, 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_last_pos_y(system.get_corpse(cursor_selected)->get_pos_y() - temp_velocity_y);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_last_pos_y(system.get_corpse(body_cursor)->get_pos_y() - temp_velocity_y);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::Dummy(ImVec2(0.0f, 5.0f));

                        /* Corpse Friction */
                        if (ImGui::DragFloat(label_fric, &temp_friction, 0.01f, 0.1f, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_friction(temp_friction);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_friction(temp_friction);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        /* Corpse Mass */
                        if (ImGui::DragFloat(label_mass, &temp_mass, 0.01f, 0.01f, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_mass(temp_mass);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_mass(temp_mass);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        /* Corpse Damping */
                        if (ImGui::DragFloat(label_damp, &temp_damping, 0.01f, 0.1f, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_damping(temp_damping);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_damping(temp_damping);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::Dummy(ImVec2(0.0f, 5.0f));

                        /* Corpse Fixed */
                        if (ImGui::TriCheckBox(label_fix, &temp_fixed)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_fixed(temp_fixed != 0);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_fixed(temp_fixed != 0);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::SameLine();

                        /* Corpse Etherial */
                        if (ImGui::TriCheckBox(label_eth, &temp_etherial)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_etherial(temp_etherial != 0);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_etherial(temp_etherial != 0);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::SameLine();

                        /* Corpse Tied */
                        if (ImGui::TriCheckBox(label_tie, &temp_tied)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_tied(temp_tied != 0);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_tied(temp_tied != 0);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::Dummy(ImVec2(0.0f, 5.0f));
                        /* Corpse Rotation */
                        if (ImGui::DragFloat(label_rot, &temp_rotation, 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->Turn(temp_rotation);
                                system.get_corpse(cursor_selected)->Bloc();
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->Turn(temp_rotation);
                                    system.get_corpse(body_cursor)->Bloc();
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        ImGui::Dummy(ImVec2(0.0f, 5.0f));

                        /* Corpse Spin */
                        if (ImGui::DragFloat(label_spi, &temp_spin, 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_last_rotation(system.get_corpse(cursor_selected)->get_rotation() - temp_spin);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_last_rotation(system.get_corpse(body_cursor)->get_rotation() - temp_spin);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                        /* Corpse Motor */
                        /*
                        if (ImGui::DragFloat(label_mot, &temp_motor, 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
                            if (unique_selected) {
                                system.get_corpse(cursor_selected)->set_motor(system.get_corpse(cursor_selected)->get_motor() - temp_motor);
                            } else {
                                for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                                    int body_cursor = selected_corpses_cursor[i];
                                    system.get_corpse(body_cursor)->set_motor(system.get_corpse(body_cursor)->get_motor() - temp_motor);
                                }
                            }
                        }
                        if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }
                        */
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }

                /* SPECIFIC PROPERTIES */
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Specific Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    if (unique_selected) {
                        // TODO
                    } else {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::TextDisabled("Select one object to edit it's specific properties");
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Trajectories")) {
                ImGui::Dummy(ImVec2(0.0f, 7.0f));

                /* TRAJECTORY PROPERTIES */
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Trajectory Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if (!paused) {
                        ImGui::TextDisabled("<Pause the System to Enable the Trajectory Preview>");
                        ImGuiStyle& style = ImGui::GetStyle();
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_TableHeaderBg]);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.Colors[ImGuiCol_TextDisabled]);
                        ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_TableHeaderBg]);
                    } else {
                        ImGui::TextDisabled("<System Paused : Trajectory Preview Enabled>");
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    std::vector<int> corpses_indexes = {};
                    for (int i = 0; i < system.get_corpses_size(); i++) {
                        corpses_indexes.push_back(system.get_corpse(i)->get_id());  // TODO: select only non deleted corpse
                    }

                    ImGui::Checkbox("Enable the Preview", &trajectory_debug_show);
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }
                    ImGui::SameLine();
                    ImGui::Help("Trajectory previsualisation occurs only\nwhen the system is paused.");
                    ImGui::SameLine();

                    ImGui::Checkbox("Preview all bodies", &trajectory_debug_all);
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }
                    ImGui::SameLine();
                    ImGui::Help("If uncheck, the trajectory displayed\nwill be the selected body one.");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    if (ImGui::DragInt("Num Steps", &trajectory_debug_step, 1.0f, 0, +INT_MAX, "%d", ImGuiSliderFlags_None)) { this->debug_system_edited = true; }
                    if (ImGui::DragInt("Time Steps", &trajectory_debug_time, 1.0f, 1, +INT_MAX, "%d", ImGuiSliderFlags_None)) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    char chr_default[30];
                    ImFormatString(chr_default, IM_ARRAYSIZE(chr_default), "<not relative to a body>");
                    const char* current_body_item = chr_default;
                    if (trajectory_debug_relative_index > 0) { current_body_item = std::to_string(corpses_indexes[trajectory_debug_relative_index - 1]).c_str(); }
                    ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

                    ImGuiStyle& style = ImGui::GetStyle();
                    float w = ImGui::CalcItemWidth();
                    float spacing = style.ItemInnerSpacing.x;
                    float button_sz = ImGui::GetFrameHeight();
                    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
                    if (ImGui::BeginCombo("##gravity", current_body_item, ImGuiComboFlags_NoArrowButton)) {
                        bool default_selected = (0 == trajectory_debug_relative_index);
                        if (ImGui::Selectable(chr_default, default_selected)) { trajectory_debug_relative_index = 0; }
                        if (default_selected) { ImGui::SetItemDefaultFocus(); }

                        for (int i = 1; i < corpses_indexes.size() + 1; i++) {
                            const char* chr_index = std::to_string(corpses_indexes[i - 1]).c_str();

                            bool is_selected = (i == trajectory_debug_relative_index);
                            if (ImGui::Selectable(chr_index, is_selected)) { trajectory_debug_relative_index = i; }
                            if (is_selected) { ImGui::SetItemDefaultFocus(); }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::PopItemWidth();
                    ImGui::SameLine(0, spacing);
                    if (ImGui::ArrowButton("##l", ImGuiDir_Left)) {
                        if (trajectory_debug_relative_index > 0) { trajectory_debug_relative_index--; }
                    }
                    ImGui::SameLine(0, spacing);
                    if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
                        if (trajectory_debug_relative_index < corpses_indexes.size()) { trajectory_debug_relative_index++; }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    if (ImGui::Button("Recompute Trajectories", {200, 23})) {
                        this->trajectory_compute_manual = true;
                        this->debug_system_edited = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Checkbox("On change", &trajectory_compute_on_change)) { this->debug_system_edited = true; }
                    ImGui::SameLine();
                    ImGui::Help("The computation is performed\nevery time a change occurs on the\npaused system.");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Collisions Accuracy */
                    if (ImGui::SliderInt("Collisions", &trajectory_collision_accuracy, 1, 100, "x%d")) { this->debug_system_edited = true; }
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Constraints Accuracy */
                    if (ImGui::SliderInt("Constraints", &trajectory_constraint_accuracy, 1, 100, "x%d")) { this->debug_system_edited = true; }

                    if (!paused) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleColor(3);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }

                /* TRAJECTORY PROPERTIES */
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Trails Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    if (ImGui::DragInt("Num Steps", &trails_debug_step, 1.0f, 0, +INT_MAX, "%d", ImGuiSliderFlags_None)) {}
                    if (ImGui::DragInt("Time Steps", &trails_debug_time, 1.0f, 1, +INT_MAX, "%d", ImGuiSliderFlags_None)) {}

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::TreePop();
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

}