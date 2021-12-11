#include "../../../include/Overlay/Gui/GuiManager.hpp"

namespace ovl {

GuiManager::GuiManager() {}

bgfx::TextureHandle GuiManager::addFont(const std::string font_name, const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, float icon_pixels) {
    ImGuiIO& io = ImGui::GetIO();

    /* Setup Icons  */
    static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
    ImFontConfig icons_cfg;
    icons_cfg.MergeMode = true;
    icons_cfg.PixelSnapH = true;
    icons_cfg.GlyphMinAdvanceX = 13.0f;  // Icons monospaced

    /* Setup and add font */
    ImFontConfig font_cfg;
    ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %spx", font_name.c_str(), gmt::to_string(size_pixels).c_str());
    io.Fonts->AddFontFromMemoryCompressedTTF(compressed_ttf_data, compressed_ttf_size, size_pixels, &font_cfg);
    io.Fonts->AddFontFromMemoryCompressedTTF(IconsForkAwersome_compressed_data, IconsForkAwersome_compressed_size, icon_pixels, &icons_cfg, icons_ranges);

    /* Texture Handle */
    uint8_t* data;
    int32_t width;
    int32_t height;

    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

    return bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy(data, width * height * 4));
}

void GuiManager::Setup() {
    /* ImGui Enable Docking */
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;          // Docking
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;      // CTRL + TAB
    io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;  // Background clicks

    /* ImGui Setup Theme */
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    /* ImGui Setup Font */
    // io.Fonts->AddFontDefault();
    // io.Fonts->Clear();

    // TODO: debug textures, see https://github.com/bkaradzic/bgfx/issues/1233
    // this->roboto_default = this->addFont("RobotoDefault.ttf", Roboto_compressed_data, Roboto_compressed_size, F_DEFAULT_SIZE, I_DEFAULT_SIZE);
    // this->roboto_medium = this->addFont("RobotoMedium.ttf", Roboto_compressed_data, Roboto_compressed_size, F_MEDIUM_SIZE, I_MEDIUM_SIZE);
    // this->roboto_bigger = this->addFont("RobotoBigger.ttf", Roboto_compressed_data, Roboto_compressed_size, F_BIGGER_SIZE, I_BIGGER_SIZE);
    // this->consolas_smaller = this->addFont("Consolas.ttf", Consolas_compressed_data, Consolas_compressed_size, F_SMALLER_SIZE, I_SMALLER_SIZE);
    // this->proggy_smaller = this->addFont("ProggyClean.ttf", Proggy_compressed_data, Proggy_compressed_size, F_SMALLER_SIZE, I_SMALLER_SIZE);

    /* Theming */
    GuiTheme::ApplyTheme();

    // From https://coolors.co/1f2421-725ac1-8d86c9-eca400-dce1de ;
    // GuiTheme::SetColors(0x1F2421FF /* Background */, 0xDCE1DEFF /* Text */, 0x725AC1FF /* MainColor */, 0x8D86C9FF /* MainAccent */, 0xECA400FF /* Highlight */);
    // GuiTheme::ApplyTheme();
}

void GuiManager::SetupGuiBaseLayout() {
    ImGuiID dock_main_id = dockspace_id;
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->WorkSize);

    dockspace_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, G_BOTTOM_DOCK_SIZE, NULL, &dock_main_id);
    dockspace_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, G_LEFT_DOCK_SIZE, NULL, &dock_main_id);
    dockspace_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, G_RIGHT_DOCK_SIZE, NULL, &dock_main_id);
    dockspace_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, G_UP_DOCK_SIZE, NULL, &dock_main_id);

    ImGui::DockBuilderDockWindow("Properties", dockspace_left_id);
    ImGui::DockBuilderDockWindow("Console", dockspace_bottom_id);

    // ImGui::DockBuilderDockWindow("Bar", dockspace_up_id);

    ImGui::DockBuilderFinish(dockspace_id);
}

void GuiManager::DrawGui() {
    DrawGuiMenu();

    // ShowPopupClearSystem();

    /* We draw the Time bar */
    // DrawGuiTimeBar();

    /* We draw the side bar as an independant Window*/
    // DrawGuiSideBar();
    // DrawGuiSideContent();

    /* The Docking must be Before all the sub Windows*/
    DrawGuiDocking();

    if (show_gui_imguidemo) { ImGui::ShowDemoWindow(&show_gui_imguidemo); }
    /*
    if (show_gui_console) {
        this->console.Draw("Console", show_gui_console); }
    }
    if (show_gui_properties) { ShowGuiProperties(&show_gui_properties); }
    if (show_gui_overlay) { ShowGuiOverlay(&show_gui_overlay); }
    if (show_gui_settings) { ShowGuiSettings(&show_gui_settings); }
    if (show_gui_spawner) { ShowGuiSpawner(&show_gui_spawner); }
    if (show_gui_benchmark) { ShowGuiBenchmark(&show_gui_benchmark); }
    */
}

void GuiManager::DrawGuiDocking() {
    /* Create the Docking Space */
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + side_bar_size + side_content_size, viewport->WorkPos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x - side_bar_size - side_content_size, viewport->WorkSize.y));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;

    host_window_flags |= ImGuiWindowFlags_NoTitleBar;
    host_window_flags |= ImGuiWindowFlags_NoCollapse;
    host_window_flags |= ImGuiWindowFlags_NoResize;
    host_window_flags |= ImGuiWindowFlags_NoMove;
    host_window_flags |= ImGuiWindowFlags_NoDocking;
    host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    host_window_flags |= ImGuiWindowFlags_NoNavFocus;
    host_window_flags |= ImGuiWindowFlags_NoBackground;

    char label[32];
    ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport", viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(label, NULL, host_window_flags);
    ImGui::PopStyleVar(3);

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    dockspace_id = ImGui::GetID("DockSpace");

    if (this->reset_base_layout || ImGui::DockBuilderGetNode(dockspace_id) == NULL) {
        this->reset_base_layout = false;

        SetupGuiBaseLayout();
    }

    dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::End();
}

}  // namespace ovl