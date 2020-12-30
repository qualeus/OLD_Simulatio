#include "../include/Renderer.hpp"

void Renderer::SetupGui() {
    /* ImGui Initialisation */
    ImGui::SFML::Init(this->window);

    /* ImGui Themes and Settings */
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    /* ImGui Enable Docking */
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
    host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

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

    if (show_gui_console) {
        ImGui::ShowDemoWindow(&show_gui_console);
    }
    if (show_gui_console) {
        ShowGuiConsole(&show_gui_console);
    }
    if (show_gui_properties) {
        ShowGuiProperties(&show_gui_properties);
    }
    if (show_gui_overlay) {
        ShowGuiOverlay(&show_gui_overlay);
    }
    if (show_gui_settings) {
        ShowGuiSettings(&show_gui_settings);
    }
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
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.5f, 0.5f, 1.0f));

        ImVec2 button_sz(G_TOP_BAR_SIZE, G_TOP_BAR_SIZE);
        int buttons_count = 20;
        ImGuiStyle& style = ImGui::GetStyle();
        style.ItemSpacing = ImVec2(5.0f, 0.0f);
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

void Renderer::ShowGuiProperties(bool* p_open) {
    if (ImGui::Begin("Properties", p_open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        char system_name[32];
        char selection_name[32];
        ImFormatString(system_name, IM_ARRAYSIZE(system_name), "System");  //"System <%s>", name.c_str());
        ImFormatString(selection_name, IM_ARRAYSIZE(selection_name), "Selection <%s>", "none");
        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem(system_name)) {
                ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
                if (ImGui::TreeNode("System Settings")) {
                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    char temp_name[32];
                    ImFormatString(temp_name, IM_ARRAYSIZE(temp_name), name.c_str());
                    ImGui::InputText("Name", temp_name, IM_ARRAYSIZE(temp_name), ImGuiInputTextFlags_EnterReturnsTrue);
                    name = temp_name;
                    // ImGui::SetKeyboardFocusHere(-1);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    static float temp_force_x = system.get_force_x();
                    static float temp_force_y = system.get_force_y();
                    ImGui::DragFloat("Force X", &temp_force_x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
                    ImGui::DragFloat("Force Y", &temp_force_y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
                    system.set_force_x(temp_force_x);
                    system.set_force_y(temp_force_y);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    static int temp_dt = system.get_dt();
                    ImGui::InputInt("dt", &temp_dt);
                    system.set_dt(temp_dt);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ftn::Rectangle rect_limits = system.get_limits();
                    static float temp_limits[4] = {rect_limits.pos.x, rect_limits.pos.y, rect_limits.size.x, rect_limits.size.y};
                    ImGui::DragFloat4("Limits", temp_limits, 1.f, -FLT_MAX, +FLT_MAX, "%.f");
                    ImGui::SameLine();
                    DrawGuiHelp("Pos (x,y) => center of the rectangle\nSize (x,y) => size of the rectangle");
                    rect_limits = {sf::Vector2f(temp_limits[0], temp_limits[1]), sf::Vector2f(temp_limits[2], temp_limits[3])};
                    system.set_limits(rect_limits);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    static bool temp_gravity = system.get_gravity();
                    static float temp_LS = system.get_LS();
                    static float temp_G = system.get_G();

                    ImGui::Checkbox("Enable Gravity", &temp_gravity);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::InputFloat("LS", &temp_LS, 1.0e6, 1.0e4, "%e");
                    ImGui::SameLine();
                    DrawGuiHelp(
                        "The speed of light in vacuum, commonly denoted c.\n"
                        "Its exact value is defined as 299 792 458 m.s-1\n"
                        "second (approximately 300 000 km.s-1");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::InputFloat("G", &temp_G, 1.0e-15, 1.0e-13, "%e");
                    ImGui::SameLine();
                    DrawGuiHelp(
                        "The gravitational constant (also known as the\n"
                        "Newtonian constant of gravitation). Its value\n"
                        "is approximately 6.674×10-11 m3.kg-1.s-2");

                    system.set_gravity(temp_gravity);
                    system.set_LS(temp_LS);
                    system.set_G(temp_G);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    static bool locked_accuracy = true;
                    ImGui::Checkbox("Lock Accuracy", &locked_accuracy);
                    ImGui::SameLine();
                    DrawGuiHelp("Warning: Accuracy greatly impact the performances");

                    if (locked_accuracy) {
                        ImGuiStyle& style = ImGui::GetStyle();
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_TableHeaderBg]);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.Colors[ImGuiCol_TextDisabled]);
                    }

                    static int temp_collision_accuracy = system.get_collision_accuracy();
                    static int temp_constraint_accuracy = system.get_constraint_accuracy();

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::SliderInt("Collisions", &temp_collision_accuracy, 1, 100, "x%d");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::SliderInt("Constraints", &temp_constraint_accuracy, 1, 100, "x%d");
                    system.set_collision_accuracy(temp_collision_accuracy);
                    system.set_constraint_accuracy(temp_constraint_accuracy);

                    if (locked_accuracy) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleColor(2);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Graphical Settings")) {
                    static ImVec4 temp_background_color = ImVec4(background_color.r, background_color.g, background_color.b, 255);
                    ImGui::ColorEdit3("Background", (float*)&temp_background_color);
                    ImGui::SameLine();
                    DrawGuiHelp("Right click on the color picker\nto change its format, or even\nexport the color.");
                    background_color = sf::Color(temp_background_color.x * 255.0f, temp_background_color.y * 255.0f, temp_background_color.z * 255.0f, 255.0f);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    static int temp_max_framerate = get_max_framerate();
                    const char* items[] = {"30 Hz", "60 Hz", "120 Hz", "No Limit"};
                    static int item_current = 1;

                    switch (temp_max_framerate) {
                        case 30:
                            item_current = 0;
                            break;
                        case 60:
                            break;
                        case 120:
                            item_current = 2;
                            break;
                        default:
                            item_current = 3;
                    }
                    ImGui::Combo("Framerate", &item_current, items, IM_ARRAYSIZE(items));

                    switch (item_current) {
                        case 0:
                            set_max_framerate(30);
                            break;
                        case 1:
                            set_max_framerate(60);
                            break;
                        case 2:
                            set_max_framerate(120);
                            break;
                        default:
                            set_max_framerate(INT_MAX);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    int temp_circle_resolution = circle_resolution;
                    ImGui::InputInt("Circle", &temp_circle_resolution);
                    if (temp_circle_resolution > 0) {
                        circle_resolution = temp_circle_resolution;
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    ImGui::InputInt("Outline", &outline_thickness);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    float temp_text_resolution = text_resolution;
                    ImGui::InputFloat("Text", &temp_text_resolution, 0.1f, 1.0f, "%.1fpx");
                    if (temp_text_resolution > 0) {
                        text_resolution = temp_text_resolution;
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    float temp_vector_size = vector_size;
                    ImGui::InputFloat("Vector", &temp_vector_size, 0.5f, 1.0f, "%.1fpx");
                    if (temp_vector_size > 0) {
                        vector_size = temp_vector_size;
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    int temp_arrow_size = arrow_size;
                    ImGui::InputInt("Arrow", &temp_arrow_size);
                    if (temp_arrow_size > 0) {
                        arrow_size = temp_arrow_size;
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Inputs Settings")) {
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    ImGui::Checkbox("Enable Inputs", &enable_inputs);
                    ImGui::SameLine();
                    ImGui::Checkbox("Paused", &paused);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    float temp_camera_pos[2] = {get_camera_x(), get_camera_y()};
                    float temp_camera_zoom = 100.0f * (100.0f / get_camera_zoom());
                    int temp_screen_size[2] = {get_screen_width(), get_screen_height()};

                    ImGui::DragFloat2("Camera", temp_camera_pos, 1.f, -FLT_MAX, +FLT_MAX, "%.f");
                    ImGui::SameLine();
                    DrawGuiHelp("Camera Position (x,y) => center of the screen");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::DragFloat("Zoom", &temp_camera_zoom, 0.5f, -FLT_MAX, +FLT_MAX, "%.1f%%");
                    ImGui::SameLine();
                    DrawGuiHelp("Camera Zoom =>  100.0f * (100.0f / Scale)");

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::DragInt2("Window", temp_screen_size, 1.f, 0, +INT_MAX, "%dpx");

                    set_camera_x(temp_camera_pos[0]);
                    set_camera_y(temp_camera_pos[1]);
                    set_camera_zoom(100.0f * (100.0f / temp_camera_zoom));
                    set_screen_width(temp_screen_size[0]);
                    set_screen_height(temp_screen_size[1]);

                    ImGui::Dummy(ImVec2(0.0f, 7.0f));

                    ImGui::SliderFloat("Lauch Power", &launch_power, -5, 5, "%.1f");
                    ImGui::SliderFloat("Zoom Speed", &zoom_speed, -1, 1, "%.2f");

                    ImGui::TreePop();
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(selection_name)) {
                ImGui::TextDisabled("Select an object to edit it's properties");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
/*
DrawText(ftn::to_string(debug_values[0]), 0, 0, 30, true, C_SUN);
DrawText("[D] Debug: " + ftn::to_string(debug_values[1]),
this->window.getSize().x - 150, 0, 24, true, C_SUN);

DrawRectangle(0, this->window.getSize().y - 35, this->window.getSize().x, 35,
true, C_BLACK);

DrawText("mouse [ " + ftn::to_string(round(debug_values[2])) + " ; " +
ftn::to_string(round(debug_values[3])) + " ]", 10, this->window.getSize().y -
30, 18, true, C_SUN); DrawText("[ " + ftn::to_string(round(debug_values[4])) +
" ; " + ftn::to_string(round(debug_values[5])) + " ]", 180,
this->window.getSize().y - 30, 18, true, C_SUN); DrawText("camera x" +
ftn::to_string(debug_values[6]), 380, this->window.getSize().y - 30, 18, true,
C_SUN); DrawText("[ " + ftn::to_string(debug_values[7]) + " ; " +
ftn::to_string(debug_values[8]) + " ]", 510, this->window.getSize().y - 30,
18, true, C_SUN);

DrawText("[r][t] dt: " + ftn::to_string(debug_values[10]),
this->window.getSize().x - 310, this->window.getSize().y - 30, 18, true,
C_SUN);

std::string string_paused = "[space] paused: false";
if (debug_values[9]) { string_paused = "[space] paused: true"; }
DrawText(string_paused, this->window.getSize().x - 180,
this->window.getSize().y - 30, 18, true, C_SUN);

std::string string_select = "";
int select_value = debug_values[11];
switch (select_value) {
case S_DEFAULT: { string_select = "[Left/Right] Default selection"; }
break; case S_SELECT_MULTIPLE: { string_select = "[Right] Multiple selection";
} break; case S_LAUNCH_CORPSE: { string_select = "[Right] Launch corpse"; }
break; case S_DRAG_CORPSE: { string_select = "[Left] Drag selection"; } break;
case S_DRAG_SCREEN: { string_select = "[Left] Drag screen"; } break;
case S_CREATE_CIRCLE: { string_select = "[Left/Right] Create circle";
} break; case S_CREATE_POLYGON: { string_select = "[Left/Right] Create
polygon"; } break;
}
DrawText(string_select, 43, 5, 20, true, C_SUN);
DrawText(ftn::to_string(debug_values[12]) + " bodies",
this->window.getSize().x - 470, this->window.getSize().y - 30, 18, true,
C_SUN);
*/

void Renderer::ShowGuiOverlay(bool* p_open) {
    const float DISTANCE = 5.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1) {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos = viewport->GetWorkPos();  // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
        ImVec2 work_area_size = viewport->GetWorkSize();
        ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE),
                                   (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    if (ImGui::Begin("Debug Overlay", p_open, window_flags)) {
        ImGui::Text("--- Debug Overlay ----------------------------");
        ImGui::SameLine();
        DrawGuiHelp("right-click to change position.");
        ImGui::Text("%.1f Frames/s (%.1f Ms/frame)", debug_values[0], (1.0f / debug_values[0]) * 10.0f);
        ImGui::Text("%.1fs since beginning - dt: %.1f", ImGui::GetTime(), debug_values[10]);
        ImGui::Text("");

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("Performances")) {
            ImGui::Separator();

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

                for (int i = IM_ARRAYSIZE(debug_frames) - 1; i > 0; i--) {
                    debug_frames[i] = debug_frames[i - 1];
                }
                debug_frames[0] = debug_values[0];
            }

            float average = 0.0f;
            static float last_limit_average = 50.0f;
            for (int n = 0; n < last_limit_average; n++) average += debug_frames[n];
            average /= last_limit_average;

            char average_text[32];
            sprintf(average_text, "-80\n\n-45\n\n-10\nAvg: %.fHz", average);
            ImGui::PlotLines(average_text, debug_frames, display_frames_size, 0, NULL, 10.0f, 80.0f, ImVec2(235.0f, 80.0f));

            ImGui::TreePop();
        }

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);  // ImGuiTreeNodeFlags_DefaultOpen
        if (ImGui::TreeNode("Mouse Position")) {
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

                last_mouse_acc = ftn::Length({io.MousePos.x - last_mouse_pos.x, io.MousePos.y - last_mouse_pos.y}) - last_mouse_vel;
                last_mouse_vel = ftn::Length({io.MousePos.x - last_mouse_pos.x, io.MousePos.y - last_mouse_pos.y});
                if (last_mouse_vel > 3.0f) {
                    mouse_angle = ftn::degree_to_radian(ftn::bearing(io.MousePos.x, io.MousePos.y, last_mouse_pos.x, last_mouse_pos.y));
                }
                last_mouse_pos = {io.MousePos.x, io.MousePos.y};
            }

            float velocity_length = rad;
            float velocity_prec = std::log(last_mouse_vel) * 8.0f;
            if (velocity_prec < rad) {
                velocity_length = velocity_prec;
            }
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

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("User Inputs")) {
            ImGui::Separator();
            ImGui::Text(" ");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Global Informations")) {
            ImGui::Separator();
            ImGui::Text("%.f corpses\n ", debug_values[12]);
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
            case 0:
                ImGui::StyleColorsDark();
                break;
            case 1:
                ImGui::StyleColorsClassic();
                break;
            case 2:
                ImGui::StyleColorsLight();
                break;
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
            if (ImGui::TreeNodeEx("...", ImGuiTreeNodeFlags_SpanFullWidth)) {
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Simulation")) {
            if (ImGui::TreeNodeEx("...", ImGuiTreeNodeFlags_SpanFullWidth)) {
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Preferences")) {
            if (ImGui::TreeNodeEx("Shortcuts", ImGuiTreeNodeFlags_SpanFullWidth)) {
                ImGui::TreePop();
            }
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
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) {
                /* Do stuff */
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Settings")) {
                show_gui_settings = true;
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Exit")) {
                Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                /* Do stuff */
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ImGui::MenuItem("Cut", "Ctrl+X")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                /* Do stuff */
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::BeginMenu("Layout")) {
                if (ImGui::MenuItem("Reset Base Layout")) {
                    this->reset_base_layout = true;
                }
                ImGui::EndMenu();
            }

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Console", NULL, &show_gui_console);
            ImGui::MenuItem("Properties", NULL, &show_gui_properties);
            ImGui::MenuItem("Debug Overlay", NULL, &show_gui_overlay);
            ImGui::MenuItem("Main menu bar", NULL, &reset_base_layout);
            ImGui::MenuItem("Main menu bar", NULL, &reset_base_layout);
            ImGui::MenuItem("Main menu bar", NULL, &reset_base_layout);
            ImGui::MenuItem("Main menu bar", NULL, &reset_base_layout);
            ImGui::MenuItem("Main menu bar", NULL, &reset_base_layout);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug")) {
            ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
            ImGui::MenuItem("Quadtree", NULL, &debug_show_quadtree);
            ImGui::MenuItem("Rectangles", NULL, &debug_show_rectangles);
            ImGui::MenuItem("Centroids", NULL, &debug_show_centroids);
            ImGui::MenuItem("Edges", NULL, &debug_show_edges);
            ImGui::MenuItem("Vertices", NULL, &debug_show_vertices);
            ImGui::MenuItem("Normals", NULL, &debug_show_normals);
            ImGui::MenuItem("Velocity", NULL, &debug_show_velocity);
            ImGui::MenuItem("XYVelocity", NULL, &debug_show_xyvelocity);
            ImGui::MenuItem("Pairs", NULL, &debug_show_pairs);
            ImGui::MenuItem("Contacts", NULL, &debug_show_contacts);
            ImGui::MenuItem("Collisions", NULL, &debug_show_collisions);
            ImGui::PopItemFlag();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Documentation")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("About")) {
                /* Do stuff */
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Renderer::DrawGuiHelp(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

struct Console {
    char InputBuf[256];
    ImVector<char*> Items;
    ImVector<const char*> Commands;
    ImVector<char*> History;
    int HistoryPos;  // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter Filter;
    bool AutoScroll;
    bool ScrollToBottom;
    const std::vector<std::pair<const ImVec4, const char*> > ColorSyntax = {{ImVec4(1.0f, 0.8f, 0.6f, 1.0f), "> "}, {ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "[error]"}};

    Console() {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        AddLog("Console Initialisation...");
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");
        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("Done");
    }

    ~Console() {
        ClearLog();
        for (int i = 0; i < History.Size; i++) free(History[i]);
    }

    // Portable helpers
    static int Stricmp(const char* s1, const char* s2) {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++;
            s2++;
        }
        return d;
    }

    static int Strnicmp(const char* s1, const char* s2, int n) {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++;
            s2++;
            n--;
        }
        return d;
    }

    static char* Strdup(const char* s) {
        size_t len = strlen(s) + 1;
        void* buf = malloc(len);
        IM_ASSERT(buf);
        return (char*)memcpy(buf, (const void*)s, len);
    }

    static void Strtrim(char* s) {
        char* str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ') str_end--;
        *str_end = 0;
    }

    void ClearLog() {
        for (int i = 0; i < Items.Size; i++) free(Items[i]);
        Items.clear();
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void Draw(const char* title, bool* p_open) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(title, p_open, ImGuiWindowFlags_None)) {  // ImGuiWindowFlags_NoFocusOnAppearing
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Close Console")) *p_open = false;
                ImGui::EndPopup();
            }

            ImGui::TextWrapped("Enter 'HELP' for help.");

            if (ImGui::SmallButton("Add Debug Text")) {
                AddLog("%d some text", Items.Size);
            }

            ImGui::SameLine();
            if (ImGui::SmallButton("Add Debug Error")) {
                AddLog("[error] something went wrong");
            }

            ImGui::SameLine();
            if (ImGui::SmallButton("Clear")) {
                ClearLog();
            }

            ImGui::SameLine();
            bool copy_to_clipboard = ImGui::SmallButton("Copy");
            ImGui::Separator();

            // Options menu
            if (ImGui::BeginPopup("Options")) {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Options, Filter
            if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
            ImGui::SameLine();
            Filter.Draw("Filter: ('-' before sentences to exclude)", 180);
            ImGui::Separator();
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear")) ClearLog();
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
            if (copy_to_clipboard) ImGui::LogToClipboard();
            for (int i = 0; i < Items.Size; i++) {
                const char* item = Items[i];
                if (!Filter.PassFilter(item)) continue;
                ImVec4 color;
                bool has_color = false;
                for (int j = 0; j < ColorSyntax.size(); j++) {
                    if (strncmp(item, ColorSyntax[j].second, 2) == 0) {
                        has_color = true;
                        color = ColorSyntax[j].first;
                        break;
                    }
                }
                if (has_color) ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color) ImGui::PopStyleColor();
            }
            if (copy_to_clipboard) ImGui::LogFinish();
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;
            ImGui::PopStyleVar();
            ImGui::EndChild();
            ImGui::Separator();

            // Command-line
            bool reclaim_focus = false;
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
            if (ImGui::InputText(" ", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this)) {
                char* s = InputBuf;
                Strtrim(s);
                if (s[0]) ExecCommand(s);
                strcpy(s, "");
                reclaim_focus = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Validate")) {
                char* s = InputBuf;
                Strtrim(s);
                if (s[0]) ExecCommand(s);
                strcpy(s, "");
            }

            // Auto-focus on window apparition
            // ImGui::SetItemDefaultFocus();
            // if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget
            ImGui::PopFont();
            ImGui::End();
        }
    }

    void ExecCommand(const char* command_line) {
        AddLog("> %s\n", command_line);
        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0) {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));
        // Process command
        if (Stricmp(command_line, "CLEAR") == 0) {
            ClearLog();
        } else if (Stricmp(command_line, "HELP") == 0) {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++) AddLog("- %s", Commands[i]);
        } else if (Stricmp(command_line, "HISTORY") == 0) {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++) AddLog("%3d: %s\n", i, History[i]);
        } else {
            AddLog("Unknown command: '%s'\n", command_line);
        }
        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
        Console* console = (Console*)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData* data) {
        switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion: {
                // Example of TEXT COMPLETION - Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf) {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
                    word_start--;
                }
                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0) candidates.push_back(Commands[i]);
                if (candidates.Size == 0) {
                    // No match
                    AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
                } else if (candidates.Size == 1) {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                } else {
                    // Multiple matches. Complete as much as we can..
                    // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                    int match_len = (int)(word_end - word_start);
                    for (;;) {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0) c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches) break;
                        match_len++;
                    }
                    if (match_len > 0) {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }
                    // List matches
                    AddLog("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++) AddLog("- %s\n", candidates[i]);
                }
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory: {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (HistoryPos == -1) HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                } else if (data->EventKey == ImGuiKey_DownArrow) {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size) HistoryPos = -1;
                }
                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos) {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};

void Renderer::ShowGuiConsole(bool* p_open) {
    static Console console;
    console.Draw("Console", p_open);
}