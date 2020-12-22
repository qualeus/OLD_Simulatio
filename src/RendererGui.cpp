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

    /* ImGui Setup Font */
    io.Fonts->AddFontDefault();
    // ImFont* roboto = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 30);

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

    dockspace_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.10f, NULL, &dock_main_id);
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_bottom_id);
    dockspace_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
    dockspace_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
    dockspace_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.10f, NULL, &dock_main_id);

    ImGui::DockBuilderDockWindow("Properties", dockspace_right_id);
    ImGui::DockBuilderDockWindow("Log", dockspace_bottom_id);
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

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Properties");
    ImGui::End();

    ImGui::Begin("Log");
    ImGui::End();

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
}

void Renderer::DrawGuiBar() {
    bool always_show = true;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(ImVec2(viewport->GetWorkSize().x, G_TOP_BAR_SIZE));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
    if (ImGui::Begin("Bar", &always_show, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
        if (ImGui::Button("test")) {
        }
        ImGui::End();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                /* Do stuff */
            }
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
        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Settings")) {
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
            if (ImGui::MenuItem("Window B")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Window C")) {
                /* Do stuff */
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug")) {
            if (ImGui::MenuItem("Display A")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Display B")) {
                /* Do stuff */
            }
            if (ImGui::MenuItem("Display C")) {
                /* Do stuff */
            }
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
        if (ImGui::MenuItem("Exit", "SPACE")) {
            Close();
        }
        ImGui::EndMainMenuBar();
    }
}
