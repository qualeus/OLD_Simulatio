#ifndef GuiManager_HPP
#define GuiManager_HPP

#include <bgfx/bgfx.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <Geometry/String.hpp>

#include "../../../assets/fonts/consolas.hpp"
#include "../../../assets/fonts/forkawersome.hpp"
#include "../../../assets/fonts/icons.hpp"
#include "../../../assets/fonts/proggy.hpp"
#include "../../../assets/fonts/roboto.hpp"
#include "../../../assets/icon/ricon.hpp"
#include "../../Serialization/FileManager.hpp"
#include "../../Serialization/Serialization.hpp"
#include "Config.hpp"
#include "GuiModule.hpp"
#include "GuiTheme.hpp"

namespace ovl {

#define G_DEBUG_MIN_FRAME_SIZE 10   // Min size of framerate array
#define G_DEBUG_MAX_FRAME_SIZE 300  // Max size of framerate array

#define G_SIDE_BAR_SIZE 60        // Size in Px
#define G_SIDE_CONTENT_SIZE 300   // Size in Px
#define G_TIME_BAR_SIZE 30        // Size in Px
#define G_HANDLE_SIZE 20          // Size in Px
#define G_OPENED_BAR_SIZE 400     // Size in Px
#define G_UP_DOCK_SIZE 0.10f      // 100% <=> 1.0f
#define G_BOTTOM_DOCK_SIZE 0.20f  // 100% <=> 1.0f
#define G_LEFT_DOCK_SIZE 0.25f    // 100% <=> 1.0f
#define G_RIGHT_DOCK_SIZE 0.25f   // 100% <=> 1.0f

#define G_ANIMATION_SPEED 10.0f

#define G_WINDOW_LABEL_SPACE 100  // remaining space for the label
#define G_WINDOW_LABEL_MIN 400    // minimum width of the widow for the label resize
#define G_WINDOW_BORDER 50        // right border of the window

#define S_MENU_INTERFACE 1
#define S_MENU_SIMULATION 2
#define S_MENU_CONSOLE 3

#define F_SMALLER_SIZE 14
#define F_DEFAULT_SIZE 16
#define F_MEDIUM_SIZE 20
#define F_BIGGER_SIZE 26

#define I_SMALLER_SIZE 16
#define I_DEFAULT_SIZE 20
#define I_MEDIUM_SIZE 30
#define I_BIGGER_SIZE 40

#define F_ROBOTO_DEFAULT 0
#define F_ROBOTO_MEDIUM 1
#define F_ROBOTO_BIGGER 2
#define F_CONSOLAS_SMALLER 3
#define F_PROGGY_SMALLER 4

#define I_MOUSE_TYPE_CURSOR 0;
#define I_MOUSE_TYPE_MOVE 1;
#define I_MOUSE_TYPE_RESIZE 2;

#define I_SIDE_MENU_DEFAULT 0
#define I_SIDE_MENU_SHAPE 1
#define I_SIDE_MENU_SPAWNER 2

class GuiManager {
   private:
    bool paused = false;
    bool reset_base_layout = false;

    bool debug_system_edited = false;
    bool locked_accuracy = true;

    phy::System* system;

    ImGui::Console console;

    ImGuiID dockspace_id;
    ImGuiID dockspace_bottom_id;
    ImGuiID dockspace_left_id;
    ImGuiID dockspace_right_id;
    ImGuiID dockspace_up_id;

    bgfx::TextureHandle roboto_default;
    bgfx::TextureHandle roboto_medium;
    bgfx::TextureHandle roboto_bigger;
    bgfx::TextureHandle consolas_smaller;
    bgfx::TextureHandle proggy_smaller;

    int input_mouse_type = I_MOUSE_TYPE_MOVE;
    int input_side_menu = I_SIDE_MENU_DEFAULT;

    std::unordered_map<std::string, bool> show_gui = {
        {"console", false},       // show_gui_console
        {"properties", false},    // show_gui_properties
        {"overlay", false},       // show_gui_overlay
        {"settings", false},      // show_gui_settings
        {"imguidemo", false},     // show_gui_imguidemo
        {"spawner", false},       // show_gui_spawner
        {"benchmark", false},     // show_gui_benchmark
        {"side_bar", false},      // show_side_bar
        {"side_content", false},  // show_side_content
        {"time_bar", false},      // show_time_bar
        {"popup_clear", false}    // show_popup_clear_system
    };

    std::unordered_map<std::string, bool> show_debug = {
        {"quadtree", false},     // show_debug_quadtree
        {"bounds", false},       // show_debug_bounds
        {"centroids", false},    // show_debug_centroids
        {"edges", false},        // show_debug_edges
        {"projections", false},  // show_debug_projections
        {"vertices", false},     // show_debug_vertices
        {"normals", false},      // show_debug_normals
        {"velocity", false},     // show_debug_velocity
        {"xyvelocity", false},   // show_debug_xyvelocity
        {"pairs", false},        // show_debug_pairs
        {"quadpairs", false},    // show_debug_quadpairs
        {"contacts", false},     // show_debug_contacts
        {"collisions", false}    // show_debug_collisions
    };

    std::unordered_map<std::string, bool> post_process = {
        {"blur", false},    // post_process_blur
        {"grid", false},    // post_process_grid
        {"gravity", false}  // post_process_gravity
    };

    float side_bar_size = show_gui["side_bar"] ? G_SIDE_BAR_SIZE : 0.0f;
    float side_content_size = show_gui["side_content"] ? G_SIDE_CONTENT_SIZE : 0.0f;
    float time_bar_size = show_gui["time_bar"] ? G_TIME_BAR_SIZE : 0.0f;

   public:
    GuiManager(phy::System* system);
    bgfx::TextureHandle addFont(const std::string font_name, const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, float icon_pixels);
    void Setup();
    void SetupGuiBaseLayout();
    void DrawGuiDocking();
    void DrawGui();

    /* GuiMenu */
    void DrawGuiMenu();
    void DrawGuiSideBar();
    void DrawGuiTimeBar();
    void ShowPopupClearSystem();
    void ClearSystem();
};

}  // namespace ovl

#endif