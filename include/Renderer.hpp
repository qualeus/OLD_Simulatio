#ifndef Renderer_HPP
#define Renderer_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <cmath>

#include "../include/Functional.hpp"
#include "../include/System.hpp"

#define C_TURQUOISE sf::Color(26, 188, 156, 255) //rgba(26, 188, 156,1.0)
#define C_GREEN sf::Color(22, 160, 133, 255) //rgba(22, 160, 133,1.0)
#define C_EMERALD sf::Color(46, 204, 113, 255) //rgba(46, 204, 113,1.0)
#define C_NEPHRITIS sf::Color(39, 174, 96, 255) //rgba(39, 174, 96,1.0)
#define C_RIVER sf::Color(52, 152, 219, 255) //rgba(52, 152, 219,1.0)
#define C_BLUE sf::Color(41, 128, 185, 255) //rgba(41, 128, 185,1.0)
#define C_AMETHYST sf::Color(155, 89, 182, 255) //rgba(155, 89, 182,1.0)
#define C_PURPLE sf::Color(142, 68, 173, 255) //rgba(142, 68, 173,1.0)
#define C_ASPHALT sf::Color(52, 73, 94, 255) //rgba(52, 73, 94,1.0)
#define C_MARINE sf::Color(44, 62, 80, 255) //rgba(44, 62, 80,1.0)
#define C_BLACK sf::Color(22, 29, 31, 255) //rgba(22, 29, 31,1.0)
#define C_SUN sf::Color(241, 196, 15, 255) //rgba(241, 196, 15,1.0)
#define C_ORANGE sf::Color(243, 156, 18, 255) //rgba(243, 156, 18,1.0)
#define C_CARROT sf::Color(230, 126, 34, 255) //rgba(230, 126, 34,1.0)
#define C_PUMPKIN sf::Color(211, 84, 0, 255) //rgba(211, 84, 0,1.0)
#define C_ALIZARIN sf::Color(231, 76, 60, 255) //rgba(231, 76, 60,1.0)
#define C_RED sf::Color(192, 57, 43, 255) //rgba(192, 57, 43,1.0)
#define C_WHITE sf::Color(236, 240, 241, 255) //rgba(236, 240, 241,1.0)
#define C_SILVER sf::Color(189, 195, 199, 255) //rgba(189, 195, 199,1.0)
#define C_CONCRETE sf::Color(149, 165, 166, 255) //rgba(149, 165, 166,1.0)
#define C_GREY sf::Color(127, 140, 141, 255) //rgba(127, 140, 141,1.0)

#define G_CIRCLE_RESOLUTION 20
#define G_OUTLINE_THICKNESS 2

#define D_DEFAULT 0
#define D_CONTACT 1
#define D_FORCES 2

#define S_DEFAULT 0
#define S_TEST 1
#define S_LAUNCH_CORPSE 2
#define S_DRAG_CORPSE 3
#define S_DRAG_SCREEN 4

class Renderer
{
private:
	sf::RenderWindow window;
	sf::View view;
	sf::Clock clock;
	sf::Time frame;

	sf::Vector2f save_pos;
	bool selected_drag_corpse_fixed;
	int selected_drag_corpse_cursor;

	std::string name;
	float mouse_x;
	float mouse_y;

	float sys_mouse_x;
	float sys_mouse_y;

	int select_type;
	int debug_type;

	float camera_x;
	float camera_y;
	float camera_zoom;
	float screen_width;
	float screen_height;
	bool paused;

	const static int DEBUG_LENGTH = 11;
	float debug_values[ DEBUG_LENGTH ] = { };

	const static int DELAY_DEBUG = 3;
	int counter_debug;

public:

	phy::System system;
	
	Renderer(std::string p_name = "Default", bool gravity = false, float force_x = 0.0f, float force_y = 0.0f, float limit_x = 4000.0f, float limit_y = 4000.0f); // Renderer Constructor
	virtual ~Renderer(); // Renderer Destructor

	void Render(); // Initialize the System and launch the Render loop
	void Close(); // Close the Renderer and Clean the System

	void Input(sf::Event event); // Handle Input events
	void UpdateMouse(); // Upate the Mouse position
	void Pause(); // Toggle the pause of the System
	void NextDebug(); // Switch the Debug type

	bool DragPositionInit(sf::Event event); // Initialize the draggig of the Position
	void DragPositionStep(sf::Event event); // Drag the position until the Release
	void DragPositionStop(sf::Event event); // Stop the dragging of the Position

	bool DragCorpseInit(sf::Event event); // Initialize the draggig of the Corpse
	void DragCorpseStep(sf::Event event); // Drag the Corpse until the Release
	void DragCorpseStop(sf::Event event); // Stop the dragging of the Corpse

	int Framerate(); // Return the number of frames per second
	void UpdateDebug(); // Update the Debug Values

	void Draw(); // Manage the drawing of the Renderer
	void DrawCorpse(std::shared_ptr<phy::Corpse> corpse);
	void DrawPair(std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair);
	void DrawQuadtree(vtr::Rectangle rect);
	void DrawLimits();

	void Debug(); // Draw the Debug objects
	void Interface(); // Draw the renderer interface

	void DebugSpeed(); // Draw the speed of the Corpses
	void DebugPairs(); // Draw the interactions of the Corpses
	void DebugDrag(); // Draw the inputs on the Corpses 

	void DrawLine(int x1, int y1, int x2, int y2, sf::Color color = sf::Color::White);
	void DrawCircle(int x, int y, int radius, sf::Color color = sf::Color::White);
	void DrawRectangle(int x, int y, int height, int width, bool fixed = false, sf::Color color = sf::Color::White, bool outline = false);
	void DrawPolygon(/* array of points, sf::Color color = sf::Color::White */ ); 
	void DrawText(std::string str, int x, int y, int size = 20, bool fixed = false, sf::Color color = sf::Color::White);

	void Camera(sf::Vector2f move, float zoom = 1.0f); // Update the positio of the Camera
	bool Paused(); // Return true if the system is paused

	// Return the mouse position on the screen
	float get_mouse_x();
	float get_mouse_y();

	// Return the select and debug types
	int get_select_type();
	int get_debug_type();

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

	// Return the pos on the plane with the pos on the screen
	sf::Vector2f get_real_pos(sf::Vector2i pos);
	float get_real_pos_x(float x);
	float get_real_pos_y(float y);

	bool rect_in_screen(vtr::Rectangle rect);
};

#endif