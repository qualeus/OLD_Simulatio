#include "../../../include/Overlay/GuiManager.hpp"

namespace ovl {

void GuiManager::ShowPopupClearSystem() {
    if (show_popup_clear_system) {
        ImGui::OpenPopup("Clear System?");
        show_popup_clear_system = false;
    }

    if (ImGui::ValidationPopup("Clear System?",
                               "Do you really want to erase all the objects in the scene ?\n"
                               "\nIncluding:\n"
                               "     * Corpses\n"
                               "     * Constraints\n"
                               "     * Spawners...\n"
                               "\nThis operation cannot be undone !")) {
        ClearSystem();
    }
}
}