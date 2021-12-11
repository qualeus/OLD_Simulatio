#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

// TODO
void GuiManager::ShowGuiOverlay(bool* p_open) {
    const float DISTANCE = 5.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[F_CONSOLAS_SMALLER]);
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
        ImGui::Help("right-click to change position.");
        ImGui::Text("%.1f Frames/s (%.1f ms/frame)", debug_values[0], (1.0f / debug_values[0]) * 1000.0f);
        ImGui::Text("%.1fs since beginning - dt: %.3f", ImGui::GetTime(), debug_values[10]);
        ImGui::Text("");

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("Performances")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));

            ImGui::SetNextItemWidth(80);
            ImGui::InputInt("ms", &update_frame_delay);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(130);
            ImGui::SliderInt("count", &display_frames_size, G_DEBUG_MIN_FRAME_SIZE, G_DEBUG_MAX_FRAME_SIZE);

            float t_perf = 0.0f;
            if (ImGui::GetTime() - t_perf > (update_frame_delay / 1000.0f)) {
                t_perf = ImGui::GetTime();
                debug_frames.push_front(debug_values[0]);
                if (debug_frames.size() > G_DEBUG_MAX_FRAME_SIZE) { debug_frames.pop_back(); }
            }

            float average = 0.0f;
            float last_limit_average = 50.0f;
            for (int n = 0; n < last_limit_average; n++) average += debug_frames[n];
            average /= last_limit_average;

            char average_text[32];
            sprintf(average_text, "-80\n\n-45\n\n-10\nAvg: %.fHz", average);

            float debug_frames_copy[G_DEBUG_MAX_FRAME_SIZE] = {};
            std::copy(debug_frames.begin(), debug_frames.end(), debug_frames_copy);
            ImGui::PlotLines(average_text, debug_frames_copy, display_frames_size, 0, NULL, 10.0f, 80.0f, ImVec2(235.0f, 80.0f));

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::TreePop();
        }

        ImGui::SetNextTreeNodeOpen(false, ImGuiCond_FirstUseEver);  // ImGuiTreeNodeFlags_DefaultOpen
        if (ImGui::TreeNode("Mouse Position")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();

            const ImVec2 p = ImGui::GetCursorScreenPos();
            float x = p.x + 240.0f;
            float y = p.y + 14.0f;
            float sz = 84.0f;
            float rad = sz * 0.5f;
            ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));      // white
            ImU32 color_sec = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // red
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Update mouse values
            float t_mouse = 0.0f;
            if (ImGui::GetTime() - t_mouse > 0.05f) {
                t_mouse = ImGui::GetTime();

                last_mouse_acc = gmt::Vector<float>::Distance(gmt::Vector<float>(io.MousePos.x, io.MousePos.y), gmt::Vector<float>(last_mouse_pos.x, last_mouse_pos.y)) - last_mouse_vel;
                last_mouse_vel = gmt::Vector<float>::Distance(gmt::Vector<float>(io.MousePos.x, io.MousePos.y), gmt::Vector<float>(last_mouse_pos.x, last_mouse_pos.y));

                if (last_mouse_vel > 3.0f) {
                    mouse_angle = gmt::degree_to_radian(gmt::Vector<float>::Bearing(gmt::Vector<float>(io.MousePos.x, io.MousePos.y), gmt::Vector<float>(last_mouse_pos.x, last_mouse_pos.y)));
                }
                last_mouse_pos = {io.MousePos.x, io.MousePos.y};
            }

            float velocity_length = rad;
            float velocity_prec = std::log(last_mouse_vel) * 8.0f;
            if (velocity_prec < rad) { velocity_length = velocity_prec; }

            draw_list->AddCircle(ImVec2(x + rad, y + rad), rad, color, 20, 2.0f);
            draw_list->AddLine(ImVec2(x + rad, y + rad), ImVec2(x + rad + std ::cos(mouse_angle) * rad, y + rad + std ::sin(mouse_angle) * rad), color, 2.0f);
            draw_list->AddLine(ImVec2(x + rad, y + rad), ImVec2(x + rad + std ::cos(mouse_angle) * velocity_length, y + rad + std ::sin(mouse_angle) * velocity_length), color_sec, 2.0f);

            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text(" * Relative: (%.f,%.f)", debug_values[4], debug_values[5]);
            ImGui::Text(" * Absolute: (%.f,%.f)", debug_values[2], debug_values[3]);
            ImGui::Text(" * Global: (%.f,%.f)", io.MousePos.x, io.MousePos.y);
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text(" * Velocity: %.f", last_mouse_vel);
            ImGui::Text(" * Acceleration: %.f", last_mouse_acc);

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

        if (ImGui::TreeNode("Drawing")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text("%i vertices in the shape buffer", this->vertices_buffer.size());
            ImGui::Text("%i vertices in the circles buffer", this->circles_buffer.size());
            ImGui::Text("%i vertices in the outlines buffer", this->outlines_buffer.size());
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text("Total: %i vertices", this->vertices_buffer.size() + this->circles_buffer.size() + this->outlines_buffer.size());
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text("Draw calls:");
            ImGui::Text(" * %i triangles", this->triangles);
            ImGui::Text(" * %i rectangles", this->rectangles);
            ImGui::Text(" * %i lines", this->lines);
            ImGui::Text(" * %i arrows", this->arrows);
            ImGui::Text(" * %i circles", this->circles);
            ImGui::Text(" * %i springs", this->springs);
            ImGui::Text(" * %i polygons", this->polygons);
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Global Informations")) {
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 7.0f));
            ImGui::Text("%.f corpses", debug_values[12]);
            ImGui::Text("%.d pairs", debug_values[13]);
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

}