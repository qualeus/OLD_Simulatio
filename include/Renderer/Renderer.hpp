#ifndef Renderer_HPP
#define Renderer_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "../../assets/fonts/consolas.hpp"
#include "../../assets/fonts/forkawersome.hpp"
#include "../../assets/fonts/icons.hpp"
#include "../../assets/fonts/proggy.hpp"
#include "../../assets/fonts/roboto.hpp"
#include "../../assets/icon/ricon.hpp"
#include "../Geometry/Geometry.hpp"
#include "../Geometry/Maths.hpp"
#include "../Geometry/String.hpp"
#include "../Geometry/Vector.hpp"
#include "../Structures/System.hpp"
#include "Config.hpp"
#include "EditorColorScheme.hpp"
#include "GuiModule.hpp"
#include "Structures.hpp"

#define C_TURQUOISE sf::Color(26, 188, 156, 255)  // rgba(26, 188, 156,1.0)
#define C_GREEN sf::Color(22, 160, 133, 255)      // rgba(22, 160, 133,1.0)
#define C_EMERALD sf::Color(46, 204, 113, 255)    // rgba(46, 204, 113,1.0)
#define C_NEPHRITIS sf::Color(39, 174, 96, 255)   // rgba(39, 174, 96,1.0)
#define C_RIVER sf::Color(52, 152, 219, 255)      // rgba(52, 152, 219,1.0)
#define C_BLUE sf::Color(41, 128, 185, 255)       // rgba(41, 128, 185,1.0)
#define C_AMETHYST sf::Color(155, 89, 182, 255)   // rgba(155, 89, 182,1.0)
#define C_PURPLE sf::Color(142, 68, 173, 255)     // rgba(142, 68, 173,1.0)
#define C_ASPHALT sf::Color(52, 73, 94, 255)      // rgba(52, 73, 94,1.0)
#define C_MARINE sf::Color(44, 62, 80, 255)       // rgba(44, 62, 80,1.0)
#define C_BLACK sf::Color(22, 29, 31, 255)        // rgba(22, 29, 31,1.0)
#define C_SUN sf::Color(241, 196, 15, 255)        // rgba(241, 196, 15,1.0)
#define C_ORANGE sf::Color(243, 156, 18, 255)     // rgba(243, 156, 18,1.0)
#define C_CARROT sf::Color(230, 126, 34, 255)     // rgba(230, 126, 34,1.0)
#define C_PUMPKIN sf::Color(211, 84, 0, 255)      // rgba(211, 84, 0,1.0)
#define C_ALIZARIN sf::Color(231, 76, 60, 255)    // rgba(231, 76, 60,1.0)
#define C_RED sf::Color(192, 57, 43, 255)         // rgba(192, 57, 43,1.0)
#define C_WHITE sf::Color(236, 240, 241, 255)     // rgba(236, 240, 241,1.0)
#define C_SILVER sf::Color(189, 195, 199, 255)    // rgba(189, 195, 199,1.0)
#define C_CONCRETE sf::Color(149, 165, 166, 255)  // rgba(149, 165, 166,1.0)
#define C_GREY sf::Color(127, 140, 141, 255)      // rgba(127, 140, 141,1.0)

#define G_DEBUG_FRAME_SIZE 300    // Size of framerate array
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

#define S_SIZE 7 /* Size of the array of Selections values */
#define S_DEFAULT 0
#define S_SELECT_MULTIPLE 1
#define S_LAUNCH_CORPSE 2
#define S_DRAG_CORPSE 3
#define S_DRAG_SCREEN 4
#define S_CREATE_CIRCLE 5
#define S_CREATE_POLYGON 6

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

class Renderer {
   private:
    int benchmark_count = 0;
    int benchmark_reset = 10000;
    
    bool benchmark_recording = true;
    bool benchmark_paused = false;
    float benchmark_timescale = 10.0f;

    int benchmark_selection_inf = 0;
    int benchmark_selection_sup = 0;

    int input_mouse_type = I_MOUSE_TYPE_MOVE;
    int input_side_menu = I_SIDE_MENU_DEFAULT;

    int display_frames_size = 170;
    int update_frame_delay = 10;

    float launch_power = 0.2f;
    float zoom_speed = 0.1f;
    float dt_step = 0.01f;

    std::string lenght_unit = "m";
    std::string mass_unit = "kg";
    std::string time_unit = "s";

    int circle_resolution = 60;
    int outline_thickness = -2;
    float line_thickness = 2.0f;
    float text_resolution = 28.0f;
    float vector_size = 40.0f;
    float velocity_size = 10.0f;
    float min_arrow_size = 10.0f;
    int arrow_size = 12;

    bool trajectory_debug_show = false;
    bool trajectory_debug_all = true;
    bool trajectory_compute_on_change = true;
    bool trajectory_compute_manual = false;
    int trajectory_debug_step = 100;
    int trajectory_debug_time = 10;
    int trajectory_debug_index = 0;
    int trajectory_debug_relative_index = 0;
    int trajectory_collision_accuracy = 1;
    int trajectory_constraint_accuracy = 1;

    int trails_debug_step = 100;
    int trails_debug_time = 10;

    sf::RenderWindow window;
    sf::View view;
    sf::Clock clock;
    sf::Time frame;

    sf::Vector2f saved_mouse_pos;
    gmt::Bounds<float> selected_area;
    std::vector<bool> selected_corpses_fixed;
    std::vector<int> selected_corpses_cursor;
    std::vector<int> selected_corpses_index;
    std::vector<sf::Vector2f> selected_corpses_diff;

    bool reset_base_layout = false;
    bool show_gui_console = false;
    bool show_gui_properties = true;
    bool show_gui_overlay = true;
    bool show_gui_settings = false;
    bool show_gui_imguidemo = false;
    bool show_gui_spawner = false;
    bool show_gui_benchmark = true;
    bool show_side_bar = true;
    bool show_side_content = false;
    bool show_time_bar = true;
    bool show_popup_clear_system = false;

    float side_bar_size = show_side_bar ? G_SIDE_BAR_SIZE : 0.0f;
    float side_content_size = show_side_content ? G_SIDE_CONTENT_SIZE : 0.0f;
    float time_bar_size = show_time_bar ? G_TIME_BAR_SIZE : 0.0f;

    bool debug_show_quadtree = false;
    bool debug_show_bounds = false;
    bool debug_show_centroids = false;
    bool debug_show_edges = false;
    bool debug_show_projections = false;
    bool debug_show_vertices = false;
    bool debug_show_normals = false;
    bool debug_show_velocity = false;
    bool debug_show_xyvelocity = false;
    bool debug_show_pairs = false;
    bool debug_show_quadpairs = false;
    bool debug_show_contacts = false;
    bool debug_show_collisions = false;

    bool debug_system_edited = false;
    bool locked_accuracy = true;

    ImGuiID dockspace_id;
    ImGuiID dockspace_bottom_id;
    ImGuiID dockspace_left_id;
    ImGuiID dockspace_right_id;
    ImGuiID dockspace_up_id;

    std::string name;
    sf::Color background_color = sf::Color(0, 0, 0, 255);
    int max_framerate = 60;

    float mouse_x;
    float mouse_y;
    float sys_mouse_x;
    float sys_mouse_y;
    int select_type;
    int debug_type;

    Spawner input_spawner;
    std::vector<Spawner> spawners;

    float camera_x = 0.0f;
    float camera_y = 0.0f;
    float camera_zoom;
    int screen_width;
    int screen_height;
    bool paused;
    bool enable_inputs;

    std::unordered_map<int, std::vector<std::pair<float, float>>> trajectories;
    std::unordered_map<int, std::vector<std::pair<float, float>>> trajectories_previews;

    const static int DEBUG_LENGTH = 50;
    float debug_values[DEBUG_LENGTH] = {};
    float debug_frames[G_DEBUG_FRAME_SIZE] = {};

    sf::Vector2f last_mouse_pos = sf::Vector2f();
    float last_mouse_vel = 0.0f;
    float last_mouse_acc = 0.0f;
    float mouse_angle = 0.0f;

    const static int DELAY_DEBUG = 3;
    int counter_debug;
    int collision_number;

    ImGui::Console console;

    std::unordered_map<int, sf::Color> corpses_colors;
    std::unordered_map<int, sf::Color> constraints_colors;

    std::vector<sf::Vertex> vertices_buffer;

   public:
    phy::System system;

    Renderer(float camera_x, float camera_y, float camera_h, float camera_w, float zoom, std::string p_name, bool gravity, float force_x, float force_y, float limit_x, float limit_y, int quadtree_max_count, int quadtree_max_depth);
    void SetupGui();
    void SetupGuiBaseLayout();
    virtual ~Renderer();

    void Render();  // Initialize the System and launch the Render loop
    void RenderGui();
    void RenderWindow();
    void Close();  // Close the Renderer and Clean the System

    void Input(sf::Event event);  // Handle Input events
    void UpdateMouse();           // Upate the Mouse position
    void UpdateCorpseDatas();     // Check if the corpses in the selection still exists
    void Pause();                 // Toggle the pause of the System

    void ClearSystem();

    void addCorpse(phy::Polygon polygon, sf::Color color);
    void addCorpse(phy::Circle circle, sf::Color color);
    std::shared_ptr<phy::Corpse> getCorpse(int index) const;

    void addConstraint(phy::Link link, sf::Color color);
    void addConstraint(phy::Spring spring, sf::Color color);
    void addConstraint(phy::Slider slider, sf::Color color);
    std::shared_ptr<phy::Constraint> getConstraint(int index) const;

    bool DragPositionInit(sf::Event event);  // Initialize the draggig of the Position
    void DragPositionStep(sf::Event event);  // Drag the position until the Release
    void DragPositionStop(sf::Event event);  // Stop the dragging of the Position

    bool SelectUniqueCorpseInit(sf::Event event);  // Initialize the selection of a unique corpse to Drag

    void UpdateSpawners();
    void StepSpawner(Spawner* spawner);

    void SelectMultipleCorpsesInit(sf::Event event);  // Initialize the selecting of multiple corpses to Drag
    void SelectMultipleCorpsesStep(sf::Event event);  // Drag the selection rectangle until release
    void SelectMultipleCorpsesStop(sf::Event event);  // Select corpses and stop the selection event

    void DragCorpsesStep(sf::Event event);  // Drag the Corpse until the Release
    void DragCorpsesStop(sf::Event event);  // Stop the dragging of the Corpse

    bool LaunchCorpseInit(sf::Event event);  // Initialize the launching of the Corpse
    void LaunchCorpseStep(sf::Event event);  // Drag the launcher until the Release
    void LaunchCorpseStop(sf::Event event);  // Launch corpse and Stop the launching event

    void ToggleOnCircle(sf::Event event);
    void ToggleOffCircle(sf::Event event);
    void CreateCircleInit(sf::Event event);
    void CreateCircleFast(sf::Event event);
    void CreateCircleStep(sf::Event event);
    void CreateCircleStop(sf::Event event);

    void ToggleOnPolygon(sf::Event event);
    void ToggleOffPolygon(sf::Event event);

    void CreatePolygonInit(sf::Event event);
    void CreatePolygonAddPoint(sf::Event event);
    void CreatePolygonStep(sf::Event event);
    void CreatePolygonStop(sf::Event event);

    int Framerate();  // Return the number of frames per second
    void UpdateMaxFramerate(int max_framerate);
    void UpdateDebug();  // Update the Debug Values

    void Draw();  // Manage the drawing of the Renderer
    void DrawCorpse(std::shared_ptr<phy::Corpse> corpse, sf::Color color);
    void DrawConstraint(std::shared_ptr<phy::Constraint> constraint, sf::Color color);
    void DrawPair(std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair);
    void DrawQuadTree(gmt::BoundsI rect);
    void DrawLimits();
    void DrawTrajectories();

    void ResetBenchmark();
    void Debug();
    void DrawInputs();
    void DrawGui();
    void DrawGuiMenu();
    void DrawGuiSideBar();
    void DrawGuiSideContent();
    void DrawGuiSideDefault();
    void DrawGuiSideShape();
    void DrawGuiSideSpawner();
    void DrawGuiTimeBar();
    void DrawGuiDocking();
    void DrawGuiHelp(const char* desc);
    bool DrawGuiCheckBox(const char* label, int* v_tristate);
    void DrawPerf(bmk::Performance* root, double time_init, double time_scale, size_t count = 0);
    void DrawPerfBox(bmk::Performance* perf, double time_init, double time_scale, size_t count = 0);

    void ShowGuiConsole(bool* p_open);
    void ShowGuiProperties(bool* p_open);
    void ShowGuiSpawner(bool* p_open);
    void ShowGuiOverlay(bool* p_open);
    void ShowGuiSettings(bool* p_open);
    void ShowGuiBenchmark(bool* p_open);
    void ShowGuiSettingsInterface();
    void ShowPopupClearSystem();

    void DebugSpeed();  // Draw the speed of the Corpses
    void DebugPairs();  // Draw the interactions of the Corpses
    void DebugDrag();   // Draw the inputs on the Corpses

    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, sf::Color color = sf::Color::White);
    void DrawLine(int x1, int y1, int x2, int y2, float thickness = 2.0f, sf::Color color = sf::Color::White);
    void DrawSpring(int x1, int y1, int x2, int y2, float thickness, int number_wave, sf::Color color);
    void DrawArrow(int x1, int y1, int x2, int y2, int xhead, int yhead, float thickness = 2.0f, sf::Color color = sf::Color::White);
    void DrawCircle(int x, int y, int radius, sf::Color color = sf::Color::White, bool outline = false);
    void DrawRectangle(int x1, int y1, int x2, int y2, bool fixed = false, sf::Color color = sf::Color::White, bool outline = false);
    void DrawPolygon(std::vector<sf::Vector2f> points, sf::Color color = sf::Color::White, bool outline = false);
    void DrawText(std::string str, int x, int y, int size = 20, bool fixed = false, sf::Color color = sf::Color::White);

    void Camera(sf::Vector2f move, float zoom = 1.0f);  // Update the positio of the Camera
    bool Paused();                                      // Return true if the system is paused

    void UpdateCamera();

    // Return the mouse position on the screen
    float get_mouse_x();
    float get_mouse_y();

    // Return the select and debug types
    int get_select_type();
    int get_debug_type();
    void set_debug_type(int i);

    // Getter/Setters of the camera X position
    float get_camera_x();
    void set_camera_x(float camera_x);

    // Getter/Setters of the camera Y position
    float get_camera_y();
    void set_camera_y(float camera_y);

    // Getter/Setters of the camera Zoom
    float get_camera_zoom();
    void set_camera_zoom(float camera_zoom);

    // Return the size of the view on the base plane
    float get_camera_size();
    void set_camera_size(float camera_size);

    int get_screen_width();
    void set_screen_width(int screen_width);

    int get_screen_height();
    void set_screen_height(int screen_height);

    int get_max_framerate();
    void set_max_framerate(int max_framerate);

    gmt::Bounds<float> get_screen_bounds();

    // Return the pos on the plane with the pos on the screen
    sf::Vector2f get_real_pos(sf::Vector2i pos);
    float get_real_pos_x(float x);
    float get_real_pos_y(float y);

    bool get_enable_inputs();
    void set_enable_inputs(bool enable);
};

#endif