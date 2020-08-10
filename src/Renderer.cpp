#include "../include/Renderer.hpp"

Renderer::Renderer(float camera_x, float camera_y, float camera_h, float camera_w, float zoom, std::string p_name, bool gravity, float force_x, float force_y, float limit_x, float limit_y) {

	// System
	this->system = phy::System(gravity, force_x, force_y, limit_x, limit_y);
	this->name = p_name;

	// Debug
	this->select_type = 0;
	this->debug_type = 0;
	this->paused = false;
	this->mouse_x = 0;
	this->mouse_y = 0;
	this->sys_mouse_x = 0;
	this->sys_mouse_y = 0;
	this->counter_debug = 0;

	this->selected_drag_corpse_fixed = false;
	this->selected_drag_corpse_cursor = -1;

	// Camera
	this->camera_zoom = 100.0f;
	this->screen_width = camera_w;
	this->screen_height = camera_h;
	this->camera_x = 0.0f;
	this->camera_y = 0.0f;

	this->view.reset(sf::FloatRect(-screen_width/2, -screen_height/2, this->screen_width, this->screen_height));
	Camera(sf::Vector2f(camera_x, camera_y), zoom);

	// Renderer
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	this->window.create(sf::VideoMode(this->screen_width, this->screen_height), this->name, sf::Style::Default, settings);
	this->window.setView(this->view);
	this->window.setFramerateLimit(60);	

}

Renderer::~Renderer() {}

void Renderer::Render() {
	while (this->window.isOpen()) {

		// System
		if (!this->Paused()) { this->system.Step(); }
		
		// Renderer
		this->window.clear(sf::Color::Black);
		
		sf::Event event;
		while (this->window.pollEvent(event)) { Input(event); }

		this->Draw();
		
		// Debug
		this->frame = this->clock.restart();
		this->Debug();
		this->window.display();
	}
}

void Renderer::Close() {
	if (this->window.isOpen()) { this->window.close(); }
}

void Renderer::Input(sf::Event event) {
	switch (this->select_type) {
		case S_DEFAULT:
			break;
		case S_TEST:
			break;
		case S_LAUNCH_CORPSE:
			break;
		case S_DRAG_CORPSE:
			DragCorpseStep(event);
			break;
		case S_DRAG_SCREEN:
			break;
	}

	if (event.type == sf::Event::Closed) {

		// Manage the Closure event
		Close();

	} else if (event.type == sf::Event::MouseButtonPressed) {

		// Handle the functions associated with the mouse clicks
		switch (event.mouseButton.button)
		{
			case sf::Mouse::Left:
				if (!DragCorpseInit(event)) { DragPositionInit(event); }
				break;
			case sf::Mouse::Right:
				break;
		}

	} else if (event.type == sf::Event::MouseMoved) {

		// Register the new position of the mouse
		UpdateMouse();

		switch (this->select_type) {
			case S_DEFAULT:
				break;
			case S_TEST:
				break;
			case S_LAUNCH_CORPSE:
				break;
			case S_DRAG_CORPSE:
				break;
			case S_DRAG_SCREEN:
				DragPositionStep(event);
				break;
		}

	} else if (event.type == sf::Event::MouseButtonReleased) {

		// Handle the functions associated with the mouse release
		switch (event.mouseButton.button)
		{
			case sf::Mouse::Left:
				DragPositionStop(event);
				DragCorpseStop(event);
				break;
			case sf::Mouse::Right:
				break;
		}

		// Stop the events associated with the mouse holding
	} else if (event.type == sf::Event::MouseWheelScrolled) {
		UpdateMouse();
		Camera(sf::Vector2f(0.0f, 0.0f), 1.0f-(event.mouseWheelScroll.delta*0.05));
	} else if (event.type == sf::Event::KeyPressed) {

		// Handle the functions associated with the keyboard buttons
		switch (event.key.code)
		{
			case sf::Keyboard::D:
				NextDebug();
				break;
			case sf::Keyboard::R:
				this->system.add_dt(-100);
				break;
			case sf::Keyboard::T:
				this->system.add_dt(100);
				break;
			case sf::Keyboard::Space:
				Pause();
				break;
		}
	}
}

void Renderer::UpdateMouse() {
	sf::Vector2i mouse_position = sf::Mouse::getPosition(this->window);
	this->mouse_x = mouse_position.x;
	this->mouse_y = mouse_position.y;

	float center_x = this->view.getCenter().x;
	float center_y = this->view.getCenter().y;

	this->sys_mouse_x = get_real_pos_x(mouse_x);
	this->sys_mouse_y = get_real_pos_y(mouse_y);
}

void Renderer::Pause() { this->paused = !this->paused; }
void Renderer::NextDebug() { if (this->debug_type < 3) { this->debug_type++; } else { this->debug_type = 0; } }
int Renderer::Framerate() { return (1000 / this->frame.asMilliseconds()); }
void Renderer::UpdateDebug() {
	debug_values[0] = Framerate(); 
	debug_values[1] = debug_type;
	debug_values[2] = this->mouse_x;
	debug_values[3] = this->mouse_y;
	debug_values[4] = this->sys_mouse_x;
	debug_values[5] = this->sys_mouse_y;
	debug_values[6] = ftn::digits_comma(1/get_camera_size(), 3);
	debug_values[7] = this->camera_x;
	debug_values[8] = this->camera_y;
	debug_values[9] = this->paused;
	debug_values[10] = this->system.get_dt();;
}

bool Renderer::DragPositionInit(sf::Event event) { 
	if (this->select_type == S_DRAG_SCREEN) { return false; } 

	this->select_type = S_DRAG_SCREEN;
	save_pos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));          
	return true;
}

void Renderer::DragPositionStep(sf::Event event) { 
	const sf::Vector2f new_pos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
	const sf::Vector2f delta_pos = this->save_pos - new_pos;

	Camera(delta_pos);

	save_pos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));                        
}

void Renderer::DragPositionStop(sf::Event event) { 
	if (this->select_type == S_DRAG_SCREEN) {
		this->select_type = S_DEFAULT;
	}
}

bool Renderer::DragCorpseInit(sf::Event event) {
	if (this->select_type == S_DRAG_CORPSE) { return false; }

	for (int i = 0; i < system.get_corpses_size(); i++) {
		if (system.get_corpse(i)->get_removed()) { continue; } // Removed

		if (system.get_corpse(i)->Pointed(this->sys_mouse_x, this->sys_mouse_y)) {
			this->selected_drag_corpse_cursor = i;

			// Fix the corpse while holding it
			this->selected_drag_corpse_fixed = system.get_corpse(i)->get_fixed();
			system.get_corpse(selected_drag_corpse_cursor)->set_fixed(true);

			this->select_type = S_DRAG_CORPSE;
			return true;
		}
	}
	return false;
}

void Renderer::DragCorpseStep(sf::Event event) {
	system.get_corpse(selected_drag_corpse_cursor)->Move(sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y), false);
	system.CorpseStop(selected_drag_corpse_cursor);
}

void Renderer::DragCorpseStop(sf::Event event) {
	if (this->select_type == S_DRAG_CORPSE) {
		this->select_type = S_DEFAULT;

		// Reset the selected corpse
		system.get_corpse(selected_drag_corpse_cursor)->set_fixed(selected_drag_corpse_fixed);
		
		system.CorpseStop(selected_drag_corpse_cursor);
		this->selected_drag_corpse_fixed = false;
		this->selected_drag_corpse_cursor = -1;
	}
}

void Renderer::Draw() {
	for (int i = 0; i < system.get_corpses_size(); i++) {
		DrawCorpse(system.get_corpse(i));
	}

	phy::Polygon* polygonA = dynamic_cast<phy::Polygon*>(system.get_corpse(1).get());
	phy::Polygon* polygonB = dynamic_cast<phy::Polygon*>(system.get_corpse(2).get());

	DrawLine(polygonA->get_pos_x(), polygonA->get_pos_y(), polygonB->get_pos_x(), polygonB->get_pos_y());

	sf::Vector2f axis = ftn::Normalize(ftn::Norme(polygonA->get_pos(), polygonB->get_pos()))*300.0f;
	sf::Vector2f pos_mid= {(polygonA->get_pos_x()+polygonB->get_pos_x())/2, (polygonA->get_pos_y()+polygonB->get_pos_y())/2};
	DrawLine(pos_mid.x, pos_mid.y, pos_mid.x+axis.x, pos_mid.y+axis.y, sf::Color::Red);
	/*
	std::vector<float> self_projections = std::vector<float>();
	std::vector<float> other_projections = std::vector<float>();

	for (int i=0; i<polygonA->get_points_number(); i++) { self_projections.push_back(ftn::Dot(polygonA->get_points().at(i), axis)); }
	for (int i=0; i<polygonB->get_points_number(); i++) { other_projections.push_back(ftn::Dot(polygonB->get_points().at(i), axis)); }

	const auto self_minmax = std::minmax_element(self_projections.begin(), self_projections.end());
	const auto other_minmax = std::minmax_element(other_projections.begin(), other_projections.end());
	
	float self_min = *self_minmax.first;
	float self_max = *self_minmax.second;
	float other_min = *other_minmax.first;
	float other_max = *other_minmax.second;

	sf::Vector2f proj = ftn::Segment_Projection(axis, pos_mid+axis, polygonA->get_points().at(0));
	DrawCircle(proj.x, proj.y, 10);
	DrawLine(polygonA->get_points().at(0).x, polygonA->get_points().at(0).y, proj.x, proj.y, sf::Color::Red);
	*/
	//std::cout << (pos_mid.x+axis.x)*self_min <<";"<< self_min << std::endl;
	
	DrawLimits();
}

void Renderer::DrawCorpse(std::shared_ptr<phy::Corpse> corpse) {
	if (corpse->get_removed()) { return; } // Removed

    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(corpse.get())) {
		DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), circle->get_color()); 
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(corpse.get())) {
    	//ftn::Rectangle bounds = polygon->get_corpse_bounds();
    	//DrawRectangle(bounds.pos.x, bounds.pos.y, bounds.size.x, bounds.size.y);

    	DrawPolygon(polygon->get_points(), polygon->get_color());
    	DrawCircle(polygon->get_pos_x(), polygon->get_pos_y(), 10, sf::Color::Red);

    	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = polygon->get_sides();
    	for (int i=0; i<sides.size(); i++) { DrawLine(sides.at(i).first.x, sides.at(i).first.y,sides.at(i).second.x, sides.at(i).second.y, sf::Color::Blue); }
    }
}

void Renderer::DrawPair(std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair) {
	if (pair.first->get_removed() || pair.second->get_removed()) { return; } // Removed
	
	sf::Vector2f pos_A = pair.first->get_pos();
	sf::Vector2f pos_B = pair.second->get_pos();

	DrawLine(pos_A.x, pos_A.y, pos_B.x, pos_B.y);
}

void Renderer::DrawQuadtree(ftn::Rectangle rect) {
	DrawRectangle(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y, false, C_RED, true);
}

void Renderer::DrawLimits() { 
	ftn::Rectangle limits = system.get_limits();
	DrawRectangle(limits.pos.x, limits.pos.y, limits.size.x, limits.size.y, false, C_RED, true); 
}

void Renderer::Debug() {
	counter_debug++;

	if(counter_debug > DELAY_DEBUG) {
		counter_debug = 0;
		UpdateDebug();
	}

	switch (this->debug_type) {
		case D_DEFAULT:
			break;
		case D_CONTACT: {
			std::vector<ftn::Rectangle> quadtrees = this->system.get_quadtree()->get_all_bounds();
			for (int i = 0; i < quadtrees.size(); i++) { DrawQuadtree(quadtrees.at(i)); }
		} break;

		case D_FORCES: {
			// for (int i = 0; i < system.get_pairs_size(); i++) { DrawPair(system.get_pair(i)); }
			std::vector<ftn::Rectangle> quadtrees = this->system.get_quadtree()->get_all_bounds();
			for (int i = 0; i < quadtrees.size(); i++) { DrawQuadtree(quadtrees.at(i)); }
			for (int i = 0; i < system.get_quad_pairs_size(); i++) { DrawPair(system.get_quad_pair(i)); }
		} break;
	}
	
	Interface();
	DrawTexts();
}

void Renderer::Interface() {

	DrawText(ftn::to_string(debug_values[0]), 0, 0, 30, true, C_SUN);
	DrawText("[D] Debug: " + ftn::to_string(debug_values[1]), this->window.getSize().x - 150, 0, 24, true, C_SUN);

	DrawRectangle(0, this->window.getSize().y - 35, this->window.getSize().x, 35, true, C_BLACK);

	DrawText("mouse [ " + ftn::to_string(round(debug_values[2])) + " ; " + ftn::to_string(round(debug_values[3])) + " ]", 10, this->window.getSize().y - 30, 18, true, C_SUN);
	DrawText("[ " + ftn::to_string(round(debug_values[4])) + " ; " + ftn::to_string(round(debug_values[5])) + " ]", 180, this->window.getSize().y - 30, 18, true, C_SUN);
	DrawText("camera x" +  ftn::to_string(debug_values[6]), 380, this->window.getSize().y - 30, 18, true, C_SUN);
	DrawText("[ " + ftn::to_string(debug_values[7]) + " ; " + ftn::to_string(debug_values[8]) + " ]", 510, this->window.getSize().y - 30, 18, true, C_SUN);
	
	DrawText("[r][t]dt: " + ftn::to_string(debug_values[10]), this->window.getSize().x - 310, this->window.getSize().y - 30, 18, true, C_SUN);
	
	if (debug_values[9]) {
		DrawText("[space] paused: true", this->window.getSize().x - 180, this->window.getSize().y - 30, 18, true, C_SUN);
	} else {
		DrawText("[space] paused: false", this->window.getSize().x - 180, this->window.getSize().y - 30, 18, true, C_SUN);
	}

}

void Renderer::DrawLine(int x1, int y1, int x2, int y2, sf::Color color) {

    // test if the line is in the screen bounds (TODO test if the line pass by the rect for screen for the zoom)
	if (((x1 > get_real_pos_x(0)) && (x1 < get_real_pos_x(screen_width)) && (y1 > get_real_pos_y(0)) && (y1 < get_real_pos_y(screen_height))) || ((x2 > get_real_pos_x(0)) && (x2 < get_real_pos_x(screen_height)) && (y2 > get_real_pos_y(0)) && (y2 < get_real_pos_y(screen_height)))) {
		sf::RectangleShape line(sf::Vector2f(ftn::Length(x1, y1, x2, y2), 5));
		line.setOrigin(0, 2);
		line.setPosition(x2, y2);
		line.rotate(ftn::bearing(x1, y1, x2, y2));
		line.setFillColor(color);
		this->window.draw(line);
	}
}

void Renderer::DrawCircle(int x, int y, int radius, sf::Color color) {

    // test if the circle is in the screen bounds
	if (((x + radius > get_real_pos_x(0)) && (x - radius < get_real_pos_x(screen_width)) && (y + radius > get_real_pos_y(0)) && (y - radius < get_real_pos_y(screen_height))) || ((x > get_real_pos_x(0)) && (x < get_real_pos_x(screen_width)) && (y > get_real_pos_y(0)) && (y < get_real_pos_y(screen_height)))) {
		sf::CircleShape circle(radius, G_CIRCLE_RESOLUTION);
		circle.setPosition(x, y);
		circle.setFillColor(color);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		this->window.draw(circle);
	}
}

void Renderer::DrawRectangle(int x, int y, int width, int height, bool fixed, sf::Color color, bool outline) {
	if (fixed) {
		sf::RectangleShape rect(sf::Vector2f(width, height));
		rect.setPosition(x, y);
		rect.setFillColor(color);

		rect.scale(get_camera_size(), get_camera_size());
		rect.setPosition(get_real_pos_x(x), get_real_pos_y(y));
		
		this->window.draw(rect);
	} else {
    	// test if the rectangle is in the screen bounds
    	if (rect_in_screen({sf::Vector2f(x, y), sf::Vector2f(width, height)})) {
			sf::RectangleShape rect(sf::Vector2f(width, height));
			rect.setPosition(x, y);
			if (outline) {
				rect.setOutlineThickness(G_OUTLINE_THICKNESS);
				rect.setOutlineColor(color);
				rect.setFillColor(sf::Color::Transparent);
			} else {
				rect.setFillColor(color);
			}
			this->window.draw(rect);
		}
	}
}

void Renderer::DrawPolygon(std::vector<sf::Vector2f> points, sf::Color color) {
	sf::ConvexShape convex;

	convex.setPointCount(points.size());
	for (int i=0; i<points.size(); i++) { convex.setPoint(i, points.at(i)); }
	
	convex.setFillColor(color);
	this->window.draw(convex);
}
	

void Renderer::DrawText(std::string str, int x, int y, int size, bool fixed, sf::Color color) {
	sf::Font font;
	sf::Text text;

	if (font.loadFromFile("arial.ttf")) {
		text.setCharacterSize(G_TEXT_RESOLUTION);
		float resolution_resize = size/G_TEXT_RESOLUTION;

		if (fixed) {
			text.scale(get_camera_size()*resolution_resize, get_camera_size()*resolution_resize);
			text.setPosition(get_real_pos_x(x), get_real_pos_y(y));
		} else {
			text.scale(resolution_resize, resolution_resize);
			text.setPosition(x, y);
		}

		text.setFont(font);
		text.setString(str);
		text.setFillColor(color);
		this->window.draw(text);
	}		
}

void Renderer::Camera(sf::Vector2f move, float zoom) {

	this->view.setCenter(this->view.getCenter() + move);
	this->view.zoom(zoom);

	this->camera_x = this->view.getCenter().x;
	this->camera_y = this->view.getCenter().y;
	this->camera_zoom = this->camera_zoom * zoom;

	this->window.setView(this->view);
}

bool Renderer::Paused() { return this->paused; }

float Renderer::get_mouse_x() { return this->mouse_x; }
float Renderer::get_mouse_y() { return this->mouse_y; }

int Renderer::get_select_type() { return this->select_type; }
int Renderer::get_debug_type() { return this->debug_type; }
void Renderer::set_debug_type(int i) { this->debug_type=i; }

float Renderer::get_camera_x() { return this->camera_x; }
float Renderer::get_camera_y() { return this->camera_y; }

void Renderer::set_camera_x(float camera_x) { this->camera_x = camera_x; }
void Renderer::set_camera_y(float camera_y) { this->camera_y = camera_y; }

float Renderer::get_camera_zoom() { return this->camera_zoom; }
float Renderer::get_camera_size() { return this->camera_zoom/100.0f; }
void Renderer::set_camera_zoom(float camera_zoom) { this->camera_zoom = camera_zoom; }

sf::Vector2f Renderer::get_real_pos(sf::Vector2i pos) { 
	return window.mapPixelToCoords(pos);
	// return sf::Vector2f(get_real_pos_x(pos.x), get_real_pos_y(pos.y)); 
}
float Renderer::get_real_pos_x(float x) { 
	 return window.mapPixelToCoords(sf::Vector2i(x, 0)).x;
	// return this->view.getCenter().x + (this->camera_x + x - this->view.getCenter().x - (this->screen_width/2)) * get_camera_size(); 
}
float Renderer::get_real_pos_y(float y) { 
	 return window.mapPixelToCoords(sf::Vector2i(0, y)).y;
	// return this->view.getCenter().y + (this->camera_y + y - this->view.getCenter().y - (this->screen_height/2)) * get_camera_size(); 
}

bool Renderer::rect_in_screen(ftn::Rectangle rect) {
	// One point in screen
	if (rect.pos.x > get_real_pos_x(0) && rect.pos.x < get_real_pos_x(screen_width)) { return true; }
	if (rect.pos.x + rect.size.x > get_real_pos_x(0) && rect.pos.x + rect.size.x  < get_real_pos_x(screen_width)) { return true; }
	if (rect.pos.y > get_real_pos_y(0) && rect.pos.y < get_real_pos_y(screen_height)) { return true; }
	if (rect.pos.y + rect.size.y > get_real_pos_y(0) && rect.pos.y + rect.size.y < get_real_pos_y(screen_height)) { return true; }

	// Or screen in the shape
	if (rect.pos.x < get_real_pos_x(0) && rect.pos.x + rect.size.x > get_real_pos_x(screen_width) && rect.pos.y < get_real_pos_y(0) && rect.pos.y + rect.size.y > get_real_pos_y(screen_height)) { return true; }
	
	return false; // is it faster to test first for true or for false?
}

void Renderer::addText(ftn::Text txt) { this->texts.push_back(txt); }
void Renderer::DrawTexts() {
	for (int i = 0; i < this->texts.size(); i++) { 
		ftn::Text txt = this->texts.at(i);
		DrawText(txt.str, txt.x, txt.y, txt.size, txt.fixed, txt.color);
	}
}

