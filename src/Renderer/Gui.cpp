#include "../../include/Renderer/Renderer.hpp"

void Renderer::SetupGui() {
    /*
    sf::Image icon;
    icon.loadFromFile("../assets/icon/icon.ico");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    */

    /* ImGui Initialisation */
    ImGui::SFML::Init(this->window);

    /* ImGui Themes and Settings */
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    /* ImGui Enable Docking */
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // CTRL + TAB
    io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

    // io.ConfigDockingWithShift = true;

    /* ImGui Setup Theme */
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    /* ImGui Setup Font */
    io.Fonts->Clear();
    ImFontConfig roboto_cfg = ImFontConfig();
    ImFormatString(roboto_cfg.Name, IM_ARRAYSIZE(roboto_cfg.Name), "RobotoMedium.ttf, 16px");
    io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 16.0f, &roboto_cfg);

    ImFontConfig consolas_cfg = ImFontConfig();
    ImFormatString(consolas_cfg.Name, IM_ARRAYSIZE(consolas_cfg.Name), "Consolas.ttf, 14px");
    io.Fonts->AddFontFromMemoryCompressedTTF(Consolas_compressed_data, Consolas_compressed_size, 14.0f, &consolas_cfg);

    ImFontConfig proggy_cfg = ImFontConfig();
    ImFormatString(proggy_cfg.Name, IM_ARRAYSIZE(proggy_cfg.Name), "ProggyClean.ttf, 14px");
    io.Fonts->AddFontFromMemoryCompressedTTF(Proggy_compressed_data, Proggy_compressed_size, 14.0f, &proggy_cfg);
    ImGui::SFML::UpdateFontTexture();

    /* Add icons to the Font */
    /*
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f;  // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
    // io.Fonts->AddFontFromMemoryCompressedTTF(IconsForkAwersome_compressed_data, IconsForkAwersome_compressed_size, 16);
    io.Fonts->AddFontFromFileTTF("../assets/fonts/forkawesome-webfont.ttf", 13.0f, &config, icon_ranges);
    */
}

void Renderer::SetupGuiBaseLayout() {
    ImGuiID dock_main_id = dockspace_id;
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->GetWorkSize());  //->Size

    dockspace_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, G_BOTTOM_DOCK_SIZE, NULL, &dock_main_id);
    dockspace_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, G_LEFT_DOCK_SIZE, NULL, &dock_main_id);
    dockspace_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, G_RIGHT_DOCK_SIZE, NULL, &dock_main_id);
    dockspace_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, G_UP_DOCK_SIZE, NULL, &dock_main_id);

    ImGui::DockBuilderDockWindow("Properties", dockspace_left_id);
    ImGui::DockBuilderDockWindow("Console", dockspace_bottom_id);

    // ImGui::DockBuilderDockWindow("Bar", dockspace_up_id);

    ImGui::DockBuilderFinish(dockspace_id);
}

void Renderer::RenderGui() {
    ImGui::SFML::Render(window);

    /* ImGui Docking and Additionnal windows Rendering */
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Renderer::DrawGuiDocking() {
    /* Create the Docking Space */
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->GetWorkPos().x, viewport->GetWorkPos().y + G_TOP_BAR_SIZE));
    ImGui::SetNextWindowSize(ImVec2(viewport->GetWorkSize().x, viewport->GetWorkSize().y - G_TOP_BAR_SIZE));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    char label[32];
    ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport", viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(label, NULL, host_window_flags);
    ImGui::PopStyleVar(3);

    ImGuiDockNodeFlags dockspace_flags = 0;
    dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

    dockspace_id = ImGui::GetID("DockSpace");

    if (this->reset_base_layout || ImGui::DockBuilderGetNode(dockspace_id) == NULL) {
        this->reset_base_layout = false;

        SetupGuiBaseLayout();
    }

    dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::End();
}

void Renderer::DrawGui() {
    DrawGuiMenu();

    /* We draw the top bar as an independant Window*/
    DrawGuiBar();

    /* The Docking must be Before all the sub Windows*/
    DrawGuiDocking();

    if (show_gui_imguidemo) { ImGui::ShowDemoWindow(&show_gui_imguidemo); }
    if (show_gui_console) { ShowGuiConsole(&show_gui_console); }
    if (show_gui_properties) { ShowGuiProperties(&show_gui_properties); }
    if (show_gui_overlay) { ShowGuiOverlay(&show_gui_overlay); }
    if (show_gui_settings) { ShowGuiSettings(&show_gui_settings); }
    if (show_gui_spawner) { ShowGuiSpawner(&show_gui_spawner); }
}

void Renderer::DrawGuiBar() {
    bool always_show = true;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(ImVec2(viewport->GetWorkSize().x, G_TOP_BAR_SIZE));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
    if (ImGui::Begin("Bar", &always_show, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
        // ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.5f, 0.5f, 1.0f));

        ImVec2 button_sz(G_TOP_BAR_SIZE, G_TOP_BAR_SIZE);
        int buttons_count = 30;
        ImGuiStyle& style = ImGui::GetStyle();
        style.ItemSpacing = ImVec2(G_TOP_BAR_SIZE / 10.0f, 0.0f);
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        for (int n = 0; n < buttons_count; n++) {
            ImGui::PushID(n);
            ImGui::Button("", button_sz);
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;  // Expected position if next button was on same line
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) ImGui::SameLine();
            ImGui::PopID();
        }
        // ImGui::PopStyleColor(1);
        ImGui::End();
    }

    ImGui::PopStyleVar(2);
    // ImGui::PopStyleColor();
}

void Renderer::ShowGuiSpawner(bool* p_open) {
    ImGui::SetNextWindowSize({400, 370}, ImGuiCond_Appearing);
    if (ImGui::Begin("Spawner", p_open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("Corpses")) {
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                const char* input_corpse_items[] = {"Circle", "Regular Polygon", "Oval Polygon"};
                static const char* input_corpse_current_item = input_corpse_items[input_spawner.corpse_type];

                const char* input_spawn_items[] = {"Custom", "Explosion", "Conglomerate"};
                static const char* input_spawn_current_item = input_spawn_items[input_spawner.spawn_type];

                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("Spawner Properties")) {
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
                    ImGui::Text("Corpse Type");
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
                    ImGui::Text("Spawner Type");
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    float input_spawner_position[2] = {input_spawner.positionX, input_spawner.positionY};
                    if (ImGui::DragFloat2("Spawner position", input_spawner_position)) {
                        input_spawner.positionX = input_spawner_position[0];
                        input_spawner.positionY = input_spawner_position[1];
                    }
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragInt("corpses number", &input_spawner.corpse_number, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragInt("spawner duration", &input_spawner.duration, 0.1, -1, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragInt("spawner interval", &input_spawner.interval, 0.1, 0, +INT_MAX)) {}
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
                    static ImVec4 temp_spawner_color = ImVec4(input_spawner.corpse_color[0], input_spawner.corpse_color[1], input_spawner.corpse_color[2], input_spawner.corpse_color[3]);
                    if (ImGui::ColorEdit3("Color", (float*)&temp_spawner_color)) {
                        input_spawner.corpse_color[0] = temp_spawner_color.x * 255.0f;
                        input_spawner.corpse_color[1] = temp_spawner_color.y * 255.0f;
                        input_spawner.corpse_color[2] = temp_spawner_color.z * 255.0f;
                        input_spawner.corpse_color[3] = temp_spawner_color.w;
                    }

                    if (input_spawner.corpse_type != 0) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragInt("points number", &input_spawner.corpse_points_number, 0.1, 3, +INT_MAX)) {}
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("corpses randiusX", &input_spawner.corpse_radiusX, 0.1, 0, +INT_MAX)) {}

                    if (input_spawner.corpse_type == 2) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("corpses randiusY", &input_spawner.corpse_radiusY, 0.1, 0, +INT_MAX)) {}
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("radius randomX", &input_spawner.corpse_radius_randomX, 0.1, 0, +INT_MAX)) {}

                    if (input_spawner.corpse_type == 2) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("radius randomY", &input_spawner.corpse_radius_randomY, 0.1, 0, +INT_MAX)) {}
                    }

                    if (input_spawner.spawn_type != 1) {
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("positionX random", &input_spawner.corpse_position_randomX, 0.1, 0, +INT_MAX)) {}
                        ImGui::Dummy(ImVec2(0.0f, 7.0f));
                        if (ImGui::DragFloat("positionY random", &input_spawner.corpse_position_randomY, 0.1, 0, +INT_MAX)) {}
                    }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("corpse rotation", &input_spawner.corpse_rotation, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("rotation random", &input_spawner.corpse_rotation_random, 0.1, 0, +INT_MAX)) {}
                    ImGui::TreePop();
                }
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                if (ImGui::TreeNode("Launch Properties")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("launch power", &input_spawner.launch_power, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("launch random", &input_spawner.launch_random, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("launch direction", &input_spawner.launch_direction, 0.1, 0, 360)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("direction random", &input_spawner.launch_direction_random, 0.1, 0, 360)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("launch rotation", &input_spawner.launch_rotation_power, 0.1, 0, +INT_MAX)) {}
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    if (ImGui::DragFloat("rotation random", &input_spawner.launch_rotation_random, 0.1, 0, +INT_MAX)) {}
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
    }
    ImGui::End();
}

void Renderer::ShowGuiProperties(bool* p_open) {
    if (ImGui::Begin("Properties", p_open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        char system_name[32];
        char selection_name[32];
        ImFormatString(system_name, IM_ARRAYSIZE(system_name), "System");  //"System <%s>", name.c_str());
        ImFormatString(selection_name, IM_ARRAYSIZE(selection_name), "Selection");

        if (ImGui::BeginTabBar("MyTabBar")) {
            /* SYSTEM PROPERTIES */
            if (ImGui::BeginTabItem(system_name)) {
                ImGui::Dummy(ImVec2(0.0f, 1.2f));

                /* PRECISION */
                ImGui::Dummy(ImVec2(0.0f, 7.0f));
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ("PHYSICS_PRECISION:" + gmt::to_string(gmt::type_name<PHYSICS_PRECISION>())).c_str());
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
                    static float temp_force_x = system.get_force_x();
                    if (ImGui::DragFloat("Force X", &temp_force_x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) { system.set_force_x(temp_force_x); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    /* System Force_Y */
                    static float temp_force_y = system.get_force_y();
                    if (ImGui::DragFloat("Force Y", &temp_force_y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None)) { system.set_force_y(temp_force_y); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Delta Time */
                    static float temp_dt = system.get_dt();
                    if (ImGui::InputFloat("dt", &temp_dt, 0.001f, 0.1f, "%.3f")) { system.set_dt(temp_dt); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Limits */
                    gmt::BoundsI rect_limits = system.get_limits();
                    static float temp_limits[4] = {static_cast<float>(rect_limits.x1), static_cast<float>(rect_limits.y1), static_cast<float>(rect_limits.x2), static_cast<float>(rect_limits.y2)};
                    if (ImGui::DragFloat4("Limits", temp_limits, 1.f, -FLT_MAX, +FLT_MAX, "%.f")) {
                        rect_limits = gmt::BoundsI(temp_limits[0], temp_limits[1], temp_limits[2], temp_limits[3]);
                        system.set_limits(rect_limits);
                    }
                    ImGui::SameLine();
                    ImGui::Help("Pos (x,y) => center of the rectangle\nSize (x,y) => size of the rectangle");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* System Enable Gravity */
                    static bool temp_gravity = system.get_gravity();
                    if (ImGui::Checkbox("Enable Gravity", &temp_gravity)) { system.set_gravity(temp_gravity); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Light Speed */
                    static float temp_LS = system.get_LS();
                    if (ImGui::InputFloat("LS", &temp_LS, temp_LS / 1000.0f, temp_LS / 10.0f, "%e")) { system.set_LS(temp_LS); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }
                    ImGui::SameLine();
                    ImGui::Help(
                        "The speed of light in vacuum, commonly denoted c.\n"
                        "Its exact value is defined as 299 792 458 m.s-1\n"
                        "(approximately 300 000 km.s-1)");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Gravity */
                    static float temp_G = system.get_G();
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

                    static bool locked_accuracy = true;
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
                    static int temp_collision_accuracy = system.get_collision_accuracy();
                    if (ImGui::SliderInt("Collisions", &temp_collision_accuracy, 1, 100, "x%d")) { system.set_collision_accuracy(temp_collision_accuracy); }
                    if (ImGui::IsItemDeactivatedAfterChange()) { this->debug_system_edited = true; }

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* System Constraints Accuracy */
                    static int temp_constraint_accuracy = system.get_constraint_accuracy();
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
                    static ImVec4 temp_background_color = ImVec4(background_color.r, background_color.g, background_color.b, 255);
                    if (ImGui::ColorEdit3("Background", (float*)&temp_background_color)) { background_color = sf::Color(temp_background_color.x * 255.0f, temp_background_color.y * 255.0f, temp_background_color.z * 255.0f, 255.0f); }
                    ImGui::SameLine();
                    ImGui::Help("Right click on the color picker\nto change its format, or even\nexport the color.");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    /* Renderer Framerate */
                    static int temp_max_framerate = get_max_framerate();
                    const char* items[] = {"30 Hz", "60 Hz", "120 Hz", "No Limit"};
                    static int item_current = 0;

                    if (ImGui::Combo("Framerate", &item_current, items, IM_ARRAYSIZE(items))) {
                        switch (item_current) {
                            case 0: set_max_framerate(30); break;
                            case 1: set_max_framerate(60); break;
                            case 2: set_max_framerate(120); break;
                            default: set_max_framerate(INT_MAX);
                        }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Circle Resolution */
                    int temp_circle_resolution = circle_resolution;
                    if (ImGui::InputInt("Circle", &temp_circle_resolution)) {
                        if (temp_circle_resolution > 0) { circle_resolution = temp_circle_resolution; }
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    /* Renderer Outline Thickness */
                    ImGui::InputInt("Outline", &outline_thickness);

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
                static char selection_label[30];
                if (selected_corpses_cursor.size() == 0) {
                    ImFormatString(selection_label, IM_ARRAYSIZE(selection_label), "Selection <%s>", "empty");
                } else if (selected_corpses_cursor.size() == 1) {
                    ImFormatString(selection_label, IM_ARRAYSIZE(selection_label), "Selection <1 body [ID %i] >", system.get_corpse(selected_corpses_cursor.at(0))->get_id());
                } else {
                    ImFormatString(selection_label, IM_ARRAYSIZE(selection_label), "Selection <%i bodies>", selected_corpses_cursor.size());
                }
                ImGui::Text(selection_label);
                static int selected_index = 0;

                /* Selection Multiple */
                if (selected_corpses_cursor.size() > 1) {
                    ImGui::SameLine();

                    char chr_default[30];
                    ImFormatString(chr_default, IM_ARRAYSIZE(chr_default), "<%i bodies>", selected_corpses_cursor.size());
                    const char* current_item = chr_default;
                    if (selected_index > 0) { current_item = std::to_string(selected_corpses_cursor.at(selected_index - 1)).c_str(); }
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
                            const char* chr_index = std::to_string(selected_corpses_cursor.at(i - 1)).c_str();

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
                        cursor_selected = selected_corpses_cursor.at(0);
                        trajectory_debug_index = selected_corpses_cursor.at(0);
                    } else {
                        if (selected_index > 0) {
                            cursor_selected = selected_corpses_cursor.at(selected_index - 1);
                            trajectory_debug_index = selected_corpses_cursor.at(selected_index - 1);
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
                            body_cursor = selected_corpses_cursor.at(0);
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
                            body_cursor = selected_corpses_cursor.at(0);
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
                                int temp_cursor = selected_corpses_cursor.at(i);
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

                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_friction(), temp_friction, sig_com)) { ImFormatString(label_fric, IM_ARRAYSIZE(label_fric), "Fricition <differ>"); }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_mass(), temp_mass, sig_com)) { ImFormatString(label_mass, IM_ARRAYSIZE(label_mass), "Mass <differ>"); }
                                if (!gmt::float_equals(system.get_corpse(temp_cursor)->get_damping(), temp_damping, sig_com)) { ImFormatString(label_damp, IM_ARRAYSIZE(label_damp), "Damping <differ>"); }

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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                                    int body_cursor = selected_corpses_cursor.at(i);
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
                    if (trajectory_debug_relative_index > 0) { current_body_item = std::to_string(corpses_indexes.at(trajectory_debug_relative_index - 1)).c_str(); }
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
                            const char* chr_index = std::to_string(corpses_indexes.at(i - 1)).c_str();

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

                    if (ImGui::Button("Recompute Tranjectories", {200, 23})) {
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
    }
    ImGui::End();
}

void Renderer::ShowGuiOverlay(bool* p_open) {
    const float DISTANCE = 5.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1) {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos = viewport->GetWorkPos();  // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
        ImVec2 work_area_size = viewport->GetWorkSize();
        ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    if (ImGui::Begin("Debug Overlay", p_open, window_flags)) {
        ImGui::Text("--- Debug Overlay ----------------------------");
        ImGui::SameLine();
        ImGui::Help("right-click to change position.");
        ImGui::Text("%.1f Frames/s (%.1f Ms/frame)", debug_values[0], (1.0f / debug_values[0]) * 10.0f);
        ImGui::Text("%.1fs since beginning - dt: %.3f", ImGui::GetTime(), debug_values[10]);
        ImGui::Text("");

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("Performances")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));

            static int display_frames_size = 170;
            static int update_frame_delay = 10;
            ImGui::SetNextItemWidth(80);
            ImGui::InputInt("ms", &update_frame_delay);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(130);
            ImGui::SliderInt("count", &display_frames_size, 10, G_DEBUG_FRAME_SIZE);

            static float t_perf = 0.0f;
            if (ImGui::GetTime() - t_perf > (update_frame_delay / 1000.0f)) {
                t_perf = ImGui::GetTime();

                for (int i = IM_ARRAYSIZE(debug_frames) - 1; i > 0; i--) { debug_frames[i] = debug_frames[i - 1]; }
                debug_frames[0] = debug_values[0];
            }

            float average = 0.0f;
            static float last_limit_average = 50.0f;
            for (int n = 0; n < last_limit_average; n++) average += debug_frames[n];
            average /= last_limit_average;

            char average_text[32];
            sprintf(average_text, "-80\n\n-45\n\n-10\nAvg: %.fHz", average);
            ImGui::PlotLines(average_text, debug_frames, display_frames_size, 0, NULL, 10.0f, 80.0f, ImVec2(235.0f, 80.0f));

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::TreePop();
        }

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);  // ImGuiTreeNodeFlags_DefaultOpen
        if (ImGui::TreeNode("Mouse Position")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();

            const ImVec2 p = ImGui::GetCursorScreenPos();
            float x = p.x + 240.0f;
            float y = p.y + 14.0f;
            static float sz = 84.0f;
            static float rad = sz * 0.5f;
            static ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            static ImU32 color_sec = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Update mouse values
            static float t_mouse = 0.0f;
            if (ImGui::GetTime() - t_mouse > 0.05f) {
                t_mouse = ImGui::GetTime();

                last_mouse_acc = gmt::Vector<float>::Distance(gmt::Vector<float>(io.MousePos.x, io.MousePos.y), gmt::Vector<float>(last_mouse_pos.x, last_mouse_pos.y)) - last_mouse_vel;
                last_mouse_vel = gmt::Vector<float>::Distance(gmt::Vector<float>(io.MousePos.x, io.MousePos.y), gmt::Vector<float>(last_mouse_pos.x, last_mouse_pos.y));

                if (last_mouse_vel > 3.0f) { mouse_angle = gmt::degree_to_radian(gmt::Vector<float>::Bearing(gmt::Vector<float>(io.MousePos.x, io.MousePos.y), gmt::Vector<float>(last_mouse_pos.x, last_mouse_pos.y))); }
                last_mouse_pos = {io.MousePos.x, io.MousePos.y};
            }

            float velocity_length = rad;
            float velocity_prec = std::log(last_mouse_vel) * 8.0f;
            if (velocity_prec < rad) { velocity_length = velocity_prec; }
            draw_list->AddCircle(ImVec2(x + rad, y + rad), rad, color, 20, 2.0f);
            draw_list->AddLine(ImVec2(x + rad, y + rad), ImVec2(x + rad + std ::cos(mouse_angle) * rad, y + rad + std ::sin(mouse_angle) * rad), color, 2.0f);
            draw_list->AddLine(ImVec2(x + rad, y + rad), ImVec2(x + rad + std ::cos(mouse_angle) * velocity_length, y + rad + std ::sin(mouse_angle) * velocity_length), color_sec, 2.0f);
            ImGui::Text(
                " \n"
                " * Relative: (%.f,%.f)\n"
                " * Absolute: (%.f,%.f)\n"
                " * Global: (%.f,%.f)\n"
                " \n"
                " * Velocity: %.f\n"
                " * Acceleration: %.f\n ",
                debug_values[4], debug_values[5], debug_values[2], debug_values[3], io.MousePos.x, io.MousePos.y, last_mouse_vel, last_mouse_acc);

            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("User Inputs")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Text(" ");
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Global Informations")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text("%.f corpses", debug_values[12]);
            ImGui::Text("%.f pairs", debug_values[13]);
            ImGui::Text("%.f quadpairs [depth: %.f]", debug_values[14], debug_values[15]);
            ImGui::Text(" * Reduction ratio: %.f %% (%f %%)", debug_values[14] / debug_values[13] * 100.0f, (debug_values[14] - debug_values[13]) / debug_values[13] * 100.0f);
            ImGui::Text(" * In cell pairs: %.f/%.f (%.f %%)", debug_values[16], debug_values[14], debug_values[16] / debug_values[14] * 100.0f);
            ImGui::Text("%.f collisions [pairs: %.f%%]", debug_values[17], debug_values[17] / debug_values[14] * 100.0f);
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::TreePop();
        }

        ImGui::Text("\n--------------------------------------------------");

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::PopFont();
    ImGui::End();
}

void Renderer::ShowGuiSettingsInterface() {
    ImGui::Text("Interface Style");

    ImGui::Dummy(ImVec2(0.0f, 7.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    static int style_idx = 0;
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
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth;
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;

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

void Renderer::DrawGuiMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) { /* Do stuff */
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Settings")) { show_gui_settings = true; }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Exit")) { Close(); }
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
                if (ImGui::MenuItem("Reset Base Layout")) { this->reset_base_layout = true; }
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

void Renderer::ShowGuiConsole(bool* p_open) { this->console.Draw("Console", p_open); }
