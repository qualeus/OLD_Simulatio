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
    io.Fonts->Clear();
    io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 16.0f);  // New Default Font
    io.Fonts->AddFontFromMemoryCompressedTTF(Consolas_compressed_data, Consolas_compressed_size, 14.0f);
    io.Fonts->AddFontFromMemoryCompressedTTF(Proggy_compressed_data, Proggy_compressed_size, 14.0f);

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

    ImGui::DockBuilderDockWindow("Properties", dockspace_right_id);
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
        // ImGui::ShowDemoWindow(&show_gui_console);
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
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_None | ImGuiWindowFlags_NoFocusOnAppearing)) {
            ImGui::End();
            return;
        }

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
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget
        ImGui::PopFont();
        ImGui::End();
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

void Renderer::ShowGuiProperties(bool* p_open) {
    ImGui::Begin("Properties", p_open, ImGuiWindowFlags_NoFocusOnAppearing);
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
        ImGui::Text("%.1f Frames/s (%.1f Ms/frame)", debug_values[0], (1.0f / debug_values[0]) * 10.0f);
        ImGui::Text("%.1fs since beginning - dt: %.1f", ImGui::GetTime(), debug_values[10]);
        ImGui::Text("");
        if (ImGui::TreeNode("Performances")) {
            ImGui::Separator();
            static int display_frames_size = 170;
            static int update_frame_delay = 10;
            ImGui::SetNextItemWidth(80);
            ImGui::InputInt("ms", &update_frame_delay);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(130);
            ImGui::SliderInt("count", &display_frames_size, 10, G_DEBUG_FRAME_SIZE);

            static float t = 0.0f;
            if (ImGui::GetTime() - t > (update_frame_delay / 1000.0f)) {
                t = ImGui::GetTime();
                for (int i = IM_ARRAYSIZE(debug_frames) - 1; i >= 0; i--) {
                    debug_frames[i + 1] = debug_frames[i];
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
        if (ImGui::TreeNode("Mouse Position")) {
            ImGui::Text(
                " * Relative(%.f,%.f)\n"
                " * Absolute(%.f,%.f)\n"
                " * Global(%.f,%.f)\n ",
                debug_values[4], debug_values[5], debug_values[2], debug_values[3], io.MousePos.x, io.MousePos.y);
            ImGui::TreePop();
        }
        ImGui::Separator();

        if (ImGui::TreeNode("temp")) {
            ImGui::Text("%.f corpses\n ", debug_values[12]);
            ImGui::TreePop();
        }
        ImGui::Text("\n(right-click to change position)");

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
            ImGui::Separator();
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
            ImGui::Separator();
            if (ImGui::BeginMenu("Layout")) {
                if (ImGui::MenuItem("Reset Base Layout")) {
                    this->reset_base_layout = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
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
        if (ImGui::MenuItem("Exit", "SPACE")) {
            Close();
        }
        ImGui::EndMainMenuBar();
    }
}
