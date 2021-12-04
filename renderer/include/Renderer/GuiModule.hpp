#ifndef GuiModule_HPP
#define GuiModule_HPP

#include "../Geometry/Maths.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {

float AnimationLinear(bool direction, float value, float min, float max, float speed);
float AnimationEaseInOut(bool direction, float value, float min, float max, float speed);
void TextCenter(std::string text);
bool ValidationPopup(std::string title, std::string content);

void Help(const char* desc);
bool TriCheckBox(const char* label, int* v_tristate);
bool PushButton(const char* str_id, bool* v);
bool ToggleButton(const char* str_id, bool* v);

/* Console Struct */
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
        for (int i = 0; i < Items.Size; i++) { free(Items[i]); }
        Items.clear();
        Items.empty();
    }

    void Log(const std::string str) { AddLog(str.c_str()); }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
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

            if (ImGui::SmallButton("Add Debug Text")) { AddLog("%d some text", Items.Size); }

            ImGui::SameLine();
            if (ImGui::SmallButton("Add Debug Error")) { AddLog("[error] something went wrong"); }

            ImGui::SameLine();
            if (ImGui::SmallButton("Clear")) { ClearLog(); }

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
                if (ImGui::Selectable("Clear")) { ClearLog(); }
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
            if (copy_to_clipboard) { ImGui::LogToClipboard(); }

            ImGuiListClipper clipper(Items.Size);
            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                    const char* item = Items[i];
                    if (!Filter.PassFilter(item)) { continue; }
                    ImVec4 color = ImColor(255, 255, 255);
                    for (int j = 0; j < ColorSyntax.size(); j++) {
                        if (strncmp(item, ColorSyntax[j].second, 2) == 0) {
                            color = ColorSyntax[j].first;
                            break;
                        }
                    }
                    ImGui::PushStyleColor(ImGuiCol_Text, color);

                    // char temp_text[200] = "";
                    // ImFormatString(temp_text, IM_ARRAYSIZE(temp_text), "[%i] %s", i, item);
                    ImGui::TextUnformatted(item);
                    ImGui::PopStyleColor();
                }
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
}  // namespace ImGui
#endif