#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

// TODO
void GuiManager::ShowGuiBenchmark(bool* p_open) {
    if (ImGui::Begin("Benchmark - Telemetry", p_open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        if (ImGui::Button(benchmark_recording ? "Reset" : "Record")) {
            if (benchmark_recording) {
                bmk::Recorder::root.Reset();
                benchmark_paused = false;
            }

            benchmark_recording = !benchmark_recording;
        }

        ImGui::SameLine();

        if (ImGui::Button(benchmark_paused ? "Resume" : "Pause")) { benchmark_paused = !benchmark_paused; }

        ImGui::SameLine();

        if (ImGui::SliderFloat("TimeScale", &benchmark_timescale, 0.1f, 20.0f, "%.1f")) {}

        int timeline_height = 100;
        ImVec2 timelineAreaSize();
        ImGui::BeginChild("Timeline", ImVec2(0, timeline_height), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

        int max_element = static_cast<int>(ImGui::GetContentRegionAvailWidth() / 5.0f);
        int size = bmk::Recorder::root.childs.size();
        console.Log("size: " + gmt::to_string(size));
        int current_size = std::min(size, max_element);
        int bar_width = 5;

        // List of frames
        ImVec2 cursor = ImGui::GetCursorScreenPos();
        for (int i = 0; i < current_size; i++) {
            ImVec2 position = ImVec2(cursor.x + i * bar_width, cursor.y + 100);
            int index = size > max_element ? size - max_element + i : i;

            int height = static_cast<int>(bmk::Recorder::root.childs[index].Time() * 2);

            ImGui::GetWindowDrawList()->AddRectFilled(position, ImVec2(position.x + bar_width - 1, position.y - height), IM_COL32(255, 255, 255, 255));
        }

        // Dragging selection area
        ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos());
        ImGui::InvisibleButton("", ImVec2(ImGui::GetContentRegionAvailWidth(), timeline_height));
        int mouse_dragging_button = 0;
        if (ImGui::IsItemActive()) {
            int mouse_pos = static_cast<int>(ImGui::GetIO().MousePos.x - ImGui::GetWindowPos().x);
            mouse_pos = std::max(0, mouse_pos);
            mouse_pos = std::min(current_size * 5, mouse_pos);

            int selection_index = static_cast<int>(mouse_pos / bar_width);
            if (ImGui::IsMouseClicked(mouse_dragging_button)) {
                benchmark_selection_inf = selection_index;
                benchmark_selection_sup = selection_index;
            }

            if (ImGui::IsMouseDragging(mouse_dragging_button)) { benchmark_selection_sup = selection_index; }
        }

        // TODO: fix crash when resite the window ??? area selected still overflows

        // Avoid the overflow in case of reset
        if (benchmark_selection_inf > size) benchmark_selection_inf = size;
        if (benchmark_selection_sup > size) benchmark_selection_sup = size;

        int selection_inf = std::min(benchmark_selection_inf, benchmark_selection_sup);
        int selection_sup = std::max(benchmark_selection_inf, benchmark_selection_sup);

        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x + selection_inf * 5, cursor.y), ImVec2(cursor.x + selection_sup * 5, cursor.y + timeline_height), IM_COL32(54, 85, 138, 50));

        ImGui::EndChild();

        ImGui::BeginChild("Record", ImVec2(0, 100), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

        int first_index = size > max_element ? size - max_element + selection_inf : selection_inf;

        for (int i = selection_inf; i < selection_sup; i++) {
            int index = size > max_element ? size - max_element + i : i;
            DrawPerf(&bmk::Recorder::root.childs[index], bmk::Recorder::root.childs[first_index].Beginning(), benchmark_timescale);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void Renderer::DrawPerf(bmk::Performance* root, double time_init, double time_scale, size_t count) {
    for (auto& perf : root->childs) {
        DrawPerfBox(&perf, time_init, time_scale, count + 1);
        DrawPerf(&perf, time_init, time_scale, count + 1);
    }
}

void Renderer::DrawPerfBox(bmk::Performance* perf, double time_init, double time_scale, size_t count) {
    float height = 20.0f;
    ImVec2 size = ImVec2(static_cast<float>(time_scale * perf->Time() + 1), height);
    ImVec2 pos = ImVec2(static_cast<float>(time_scale * (perf->Beginning() - time_init)), height * count);
    ImVec4 color = ImColor::HSV(count * 0.05f, 0.6f, 0.6f).Value;
    ImGui::PushStyleColor(ImGuiCol_Header, color);
    ImGui::SetCursorPos(pos);
    ImGui::Selectable(perf->data->get_name().c_str(), true, ImGuiSelectableFlags_None, size);
    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(
            "Task: %s\n"
            "Location: %s:%d\n"
            "Childrens: %d\n"
            "Beginning: %fms\n"
            "Ending: %fms\n"
            "Duration: %fms\n"
            "Size: %f\n",
            perf->data->get_name().c_str(), perf->data->get_file().c_str(), perf->data->get_line(), perf->childs.size(), perf->Beginning(), perf->Ending(), perf->Time(), size.x);
    }
}

}