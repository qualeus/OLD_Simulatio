#include "../../include/Renderer/GuiModule.hpp"

namespace ImGui {

/* Helper Hover */
void Help(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

/* Tri-State CheckBox */
bool TriCheckBox(const char* label, int* v_tristate) {
    bool ret;
    if (*v_tristate == -1) {
        ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
        bool b = false;
        ret = ImGui::Checkbox(label, &b);
        if (ret) { *v_tristate = 1; }
        ImGui::PopItemFlag();
    } else {
        bool b = (*v_tristate != 0);
        ret = ImGui::Checkbox(label, &b);
        if (ret) { *v_tristate = (int)b; }
    }
    return ret;
}
}