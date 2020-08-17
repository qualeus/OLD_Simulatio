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

	this->saved_mouse_pos = sf::Vector2f();
	this->selected_area = {sf::Vector2f(), sf::Vector2f()};
	this->selected_corpses_fixed = {};
	this->selected_corpses_cursor = {};
	this->selected_corpses_diff = {};

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
	if (event.type == sf::Event::Closed) {

		// Manage the Closure event
		Close();

	} else if (event.type == sf::Event::MouseButtonPressed) {

		// Handle the functions associated with the mouse clicks
		switch (event.mouseButton.button)
		{
			case sf::Mouse::Left:{
				if (!SelectUniqueCorpseInit(event)) { DragPositionInit(event); } // If the mouse is not on an Corpse, Drag screen
				CreatePolygonStop(event);
				CreateCircleInit(event);
			} break;
			case sf::Mouse::Right: {
				if (!LaunchCorpseInit(event)) { SelectMultipleCorpsesInit(event); } // If the mouse is not on an Corpse, Select multiple
				CreateCircleFast(event);
				CreatePolygonAddPoint(event);
				// if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(this->system.get_corpse(1).get())) { polygon->add_point(sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y)); }
			} break;	
		}

	} else if (event.type == sf::Event::MouseMoved) {

		// Register the new position of the mouse
		UpdateMouse();

		switch (this->select_type) {
			case S_DEFAULT: { } break;
			case S_SELECT_MULTIPLE: { SelectMultipleCorpsesStep(event); } break;
			case S_LAUNCH_CORPSE: { LaunchCorpseStep(event); } break;
			case S_DRAG_CORPSE: { DragCorpsesStep(event); } break;
			case S_DRAG_SCREEN: { DragPositionStep(event); } break;
			case S_CREATE_CIRCLE: { CreateCircleStep(event); } break;
			case S_CREATE_POLYGON: { CreatePolygonStep(event); } break;
		}

	} else if (event.type == sf::Event::MouseButtonReleased) {

		// Handle the functions associated with the mouse release
		switch (event.mouseButton.button)
		{
			case sf::Mouse::Left: {
				DragPositionStop(event);
				DragCorpsesStop(event);
				//CreatePolygonStop(event);
				CreateCircleStop(event);
			} break;
			case sf::Mouse::Right: {
				LaunchCorpseStop(event);
				SelectMultipleCorpsesStop(event);
			} break;
		}

		// Stop the events associated with the mouse holding
	} else if (event.type == sf::Event::MouseWheelScrolled) {
		UpdateMouse();
		Camera(sf::Vector2f(0.0f, 0.0f), 1.0f-(event.mouseWheelScroll.delta*0.05));
	} else if (event.type == sf::Event::KeyPressed) {

		// Handle the functions associated with the keyboard buttons
		switch (event.key.code) {
			case sf::Keyboard::D: { NextDebug(); } break;
			case sf::Keyboard::R: { this->system.add_dt(-100); } break;
			case sf::Keyboard::T: { this->system.add_dt(100); } break;
			case sf::Keyboard::Space: { Pause(); } break;
			case sf::Keyboard::A: { ToggleOnCircle(event); } break;
			case sf::Keyboard::Z: { ToggleOnPolygon(event); } break;
		}

	} else if (event.type == sf::Event::KeyReleased ) {
		switch (event.key.code) {
			case sf::Keyboard::A: { ToggleOffCircle(event); } break;
			case sf::Keyboard::Z: { ToggleOffPolygon(event); } break;
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
void Renderer::NextDebug() { if (this->debug_type < D_SIZE) { this->debug_type++; } else { this->debug_type = 0; } }
int Renderer::Framerate() { return (1000 / this->frame.asMilliseconds()); }
void Renderer::UpdateDebug() {
	debug_values[0] = Framerate(); 
	debug_values[1] = this->debug_type;
	debug_values[2] = this->mouse_x;
	debug_values[3] = this->mouse_y;
	debug_values[4] = this->sys_mouse_x;
	debug_values[5] = this->sys_mouse_y;
	debug_values[6] = ftn::digits_comma(1/get_camera_size(), 3);
	debug_values[7] = this->camera_x;
	debug_values[8] = this->camera_y;
	debug_values[9] = this->paused;
	debug_values[10] = this->system.get_dt();
	debug_values[11] = this->select_type;
	debug_values[12] = this->system.get_corpses_size();
}

bool Renderer::DragPositionInit(sf::Event event) {
	if (this->select_type != S_DEFAULT) { return false; }

	this->select_type = S_DRAG_SCREEN;
	this->saved_mouse_pos = get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));          
	return true;
}

void Renderer::DragPositionStep(sf::Event event) { 
	if (this->select_type != S_DRAG_SCREEN) { return; }

	const sf::Vector2f new_pos = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
	const sf::Vector2f delta_pos = this->saved_mouse_pos - new_pos;

	Camera(delta_pos);

	this->saved_mouse_pos = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));                        
}

void Renderer::DragPositionStop(sf::Event event) { 
	if (this->select_type == S_DRAG_SCREEN) { this->select_type = S_DEFAULT; }
}

bool Renderer::SelectUniqueCorpseInit(sf::Event event) {

	if (this->select_type != S_DEFAULT) { return false; }

	// If already selected by multiple selection
	if (selected_corpses_cursor.size() > 0) { 
		bool one_pointed = false;
		for (int i=0; i<selected_corpses_cursor.size(); i++) {
			if (system.get_corpse(selected_corpses_cursor.at(i))->Pointed(this->sys_mouse_x, this->sys_mouse_y)) { 
				one_pointed = true; 
				break;
			}
		}

		if (one_pointed) { 
			for (int i=0; i<selected_corpses_cursor.size(); i++) {
				int index = selected_corpses_cursor.at(i);
				this->selected_corpses_diff.push_back(system.get_corpse(index)->get_pos() - get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));	
				this->selected_corpses_fixed.push_back(system.get_corpse(index)->get_fixed());	
				system.get_corpse(index)->set_fixed(true); 
			}
			this->select_type = S_DRAG_CORPSE;
			return true; 
		}

		/* Make sure that the arrays are empty */
		this->selected_corpses_cursor = {};
		this->selected_corpses_fixed = {};
		this->selected_corpses_diff = {};
	}

	for (int i = 0; i < system.get_corpses_size(); i++) {
		if (system.get_corpse(i)->get_removed()) { continue; } // Removed
		if (system.get_corpse(i)->Pointed(this->sys_mouse_x, this->sys_mouse_y)) {

			this->selected_corpses_cursor.push_back(i);
			this->selected_corpses_diff.push_back(system.get_corpse(i)->get_pos() - get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
			// Fix the corpse while holding it
			this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
			system.get_corpse(i)->set_fixed(true);

			this->select_type = S_DRAG_CORPSE;
			return true;
		}
	}
	
	/* Make sure that the arrays are empty */
	this->selected_corpses_cursor = {};
	this->selected_corpses_fixed = {};
	this->selected_corpses_diff = {};
	return false;
}

void Renderer::SelectMultipleCorpsesInit(sf::Event event) {
	/* Make sure that the arrays are empty */
	this->selected_corpses_cursor = {};
	this->selected_corpses_fixed = {};
	//this->selected_corpses_diff = {};

	if (this->select_type != S_DEFAULT) { return; }
	this->selected_area = { get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), sf::Vector2f() };
	this->select_type = S_SELECT_MULTIPLE;
}

void Renderer::SelectMultipleCorpsesStep(sf::Event event) {
	if (this->select_type != S_SELECT_MULTIPLE) { return; }
	this->selected_area.size = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - this->selected_area.pos;
}

void Renderer::SelectMultipleCorpsesStop(sf::Event event) {
	if (this->select_type != S_SELECT_MULTIPLE) { return; }

	// Reorganize the points in a rectangle ABCD (top left point A / bottom right point C)
	ftn::Rectangle rectangle = ftn::Reorder_Rectangle(this->selected_area);

	for (int i = 0; i < system.get_corpses_size(); i++) {
		if (system.get_corpse(i)->get_removed()) { continue; } // Removed
		if (phy::Circle* circle = dynamic_cast<phy::Circle*>(system.get_corpse(i).get())) {
			if (!ftn::rect_out_bounds(circle->get_corpse_bounds(), rectangle)) {
				this->selected_corpses_cursor.push_back(i);
				this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
			}
    	} else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(system.get_corpse(i).get())) {
			if (!ftn::rect_out_bounds(polygon->get_corpse_bounds(), rectangle)) {
				this->selected_corpses_cursor.push_back(i);
			}
    	}
    }
    this->select_type = S_DEFAULT;

	/* Make sure that the arrays are empty */
	this->selected_corpses_fixed = {};
	this->selected_corpses_diff = {};
}

void Renderer::DragCorpsesStep(sf::Event event) {
	for (int i=0; i<selected_corpses_cursor.size(); i++ ) {
		if (system.get_corpse(selected_corpses_cursor.at(i))->get_removed()) { continue; } // Removed
		system.get_corpse(selected_corpses_cursor.at(i))->Move(get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) + selected_corpses_diff.at(i), false);
		system.CorpseStop(selected_corpses_cursor.at(i));
	}
}

void Renderer::DragCorpsesStop(sf::Event event) {
	if (this->select_type != S_DRAG_CORPSE) { return; }
	this->select_type = S_DEFAULT;

	for (int i=0; i<selected_corpses_cursor.size(); i++ ) { 
		if (system.get_corpse(selected_corpses_cursor.at(i))->get_removed()) { continue; } // Removed
		system.get_corpse(selected_corpses_cursor.at(i))->set_fixed(selected_corpses_fixed.at(i));
		system.CorpseStop(selected_corpses_cursor.at(i));
	}

	/* Make sure that the arrays are empty */
	this->selected_corpses_fixed = {};
	this->selected_corpses_diff = {};
}

bool Renderer::LaunchCorpseInit(sf::Event event) {

	if (this->select_type != S_DEFAULT) { return false; }

	// If already selected by multiple selection
	if (selected_corpses_cursor.size() > 0) { 
		bool one_pointed = false;
		for (int i=0; i<selected_corpses_cursor.size(); i++) {
			if (system.get_corpse(selected_corpses_cursor.at(i))->Pointed(this->sys_mouse_x, this->sys_mouse_y)) { 
				one_pointed = true;
				break;
			}
		}

		if (one_pointed) { 
			for (int i=0; i<selected_corpses_cursor.size(); i++) {
				int index = selected_corpses_cursor.at(i);
				//this->selected_corpses_diff.push_back(system.get_corpse(index)->get_pos() - get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));	
				this->selected_corpses_fixed.push_back(system.get_corpse(index)->get_fixed());	
				system.get_corpse(index)->set_fixed(true); 
			}
			this->selected_area = { get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), sf::Vector2f() };
	
			this->select_type = S_LAUNCH_CORPSE;
			return true; 
		}

		/* Make sure that the arrays are empty */
		this->selected_corpses_cursor = {};
		this->selected_corpses_fixed = {};
		this->selected_corpses_diff = {};
	}

	for (int i = 0; i < system.get_corpses_size(); i++) {
		if (system.get_corpse(i)->get_removed()) { continue; } // Removed
		if (system.get_corpse(i)->Pointed(this->sys_mouse_x, this->sys_mouse_y)) {

			this->selected_corpses_cursor.push_back(i);
			//this->selected_corpses_diff.push_back(system.get_corpse(i)->get_pos() - get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
			this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
			system.get_corpse(i)->set_fixed(true);

			this->selected_area = { system.get_corpse(i)->get_pos(), sf::Vector2f() };
	
			this->select_type = S_LAUNCH_CORPSE;
			return true;
		}
	}
	return false;
}

void Renderer::LaunchCorpseStep(sf::Event event) {
	if (this->select_type != S_LAUNCH_CORPSE) { return; }
	this->selected_area.size = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - this->selected_area.pos; 
}

void Renderer::LaunchCorpseStop(sf::Event event) {
 	if (this->select_type != S_LAUNCH_CORPSE) { return; }
	for (int i = 0; i < selected_corpses_cursor.size(); i++) {
		if (selected_corpses_fixed.at(i)) { continue; }
		sf::Vector2f diff_vector = this->selected_area.pos - (this->selected_area.pos + this->selected_area.size);
		sf::Vector2f launch_vector = ftn::Normalize(diff_vector)*ftn::Length(diff_vector)*I_LAUNCH_POWER;
		system.get_corpse(selected_corpses_cursor.at(i))->Move(launch_vector);
		system.get_corpse(selected_corpses_cursor.at(i))->set_fixed(selected_corpses_fixed.at(i));
	}

	this->select_type = S_DEFAULT;
	
	/* Make sure that the arrays are empty */
	//this->selected_corpses_cursor = {};
	this->selected_corpses_fixed = {};
	this->selected_corpses_diff = {};
}

void Renderer::ToggleOnCircle(sf::Event event) {
	if (this->select_type != S_DEFAULT) { return; }
	this->select_type = S_CREATE_CIRCLE;

	/* Make sure that the arrays are empty */
	this->selected_corpses_cursor = {};
	this->selected_corpses_fixed = {};
	this->selected_corpses_diff = {};
}

void Renderer::ToggleOffCircle(sf::Event event) {
	if (this->select_type != S_CREATE_CIRCLE) { return; }
	this->select_type = S_DEFAULT;

	/* Make sure that the arrays are empty */
	this->selected_corpses_cursor = {};
	this->selected_corpses_fixed = {};
	this->selected_corpses_diff = {};
}


void Renderer::CreateCircleFast(sf::Event event) {
	if (this->select_type != S_CREATE_CIRCLE) { return; }
	sf::Vector2f temp_pos = get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
	system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, 40, 40, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
}

void Renderer::CreateCircleInit(sf::Event event) {
	if (this->select_type != S_CREATE_CIRCLE) { return; }
	this->selected_area = { get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), sf::Vector2f() };
}
void Renderer::CreateCircleStep(sf::Event event) {
	 this->selected_area.size = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - this->selected_area.pos;
}
void Renderer::CreateCircleStop(sf::Event event) {
	if (this->select_type != S_CREATE_CIRCLE) { return; }
	sf::Vector2f temp_pos = this->selected_area.pos;
	float temp_size = ftn::Length(this->selected_area.size);
	float temp_mass = (temp_size*temp_size) * 0.1f;
	system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, temp_size, temp_mass, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
	this->selected_area = { sf::Vector2f(), sf::Vector2f() };
}

void Renderer::ToggleOnPolygon(sf::Event event) {
	if (this->select_type != S_DEFAULT) { return; }
	this->select_type = S_CREATE_POLYGON;

	/* Make sure that the arrays are empty */
	this->selected_corpses_diff = {};
}
void Renderer::ToggleOffPolygon(sf::Event event) {
	if (this->select_type != S_CREATE_POLYGON) { return; }
	this->select_type = S_DEFAULT;

	if (this->selected_corpses_diff.size() > 2) {
		phy::Polygon temp_poly = phy::Polygon({}, 10, 1, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_NEPHRITIS);
		for (int i=0; i<this->selected_corpses_diff.size(); i++) {
			temp_poly.add_point(this->selected_corpses_diff.at(i));
		}
		system.addCorpse(temp_poly);
	}
	
	/* Make sure that the arrays are empty */
	this->selected_corpses_diff = {};
}

void Renderer::CreatePolygonInit(sf::Event event) {
	if (this->select_type != S_CREATE_POLYGON) { return; }
}
void Renderer::CreatePolygonAddPoint(sf::Event event) { 
	if (this->select_type != S_CREATE_POLYGON) { return; }
	sf::Vector2f temp_pos = get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
	this->selected_corpses_diff.push_back(temp_pos);
}
void Renderer::CreatePolygonStep(sf::Event event) {}
void Renderer::CreatePolygonStop(sf::Event event) {
	if (this->select_type != S_CREATE_POLYGON) { return; }
	
	if (this->selected_corpses_diff.size() > 2) {
		phy::Polygon temp_poly = phy::Polygon({}, 10, 1, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_NEPHRITIS);
		for (int i=0; i<this->selected_corpses_diff.size(); i++) {
			temp_poly.add_point(this->selected_corpses_diff.at(i));
		}
		system.addCorpse(temp_poly);
	}
	
	/* Make sure that the arrays are empty */
	this->selected_corpses_diff = {};
}

void Renderer::Draw() {
	for (int i = 0; i < system.get_corpses_size(); i++) {
		DrawCorpse(system.get_corpse(i));
	}
	DrawLimits();
}

void Renderer::DrawCorpse(std::shared_ptr<phy::Corpse> corpse) {
	if (corpse->get_removed()) { return; } // Removed

    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(corpse.get())) {
    	switch (this->debug_type) {
    		case D_DEFAULT: { } break;
    		case D_QUADTREE: { } break;
    		case D_BOUNDINGS: {
    			ftn::Rectangle bounds = circle->get_corpse_bounds();
    			DrawRectangle(bounds.pos.x, bounds.pos.y, bounds.size.x, bounds.size.y, false, sf::Color::Red, true);
    			DrawCircle(circle->get_pos_x(), circle->get_pos_y(), 5, sf::Color::Red, true);
    		} break;
    		case D_COLLISIONS: { } break;
    		case D_NORMALS: { } break;
    		case D_FORCES: { } break;
    		case D_PAIRS: { } break;
    	}

    	/* -------------------------------------- Default Drawing -------------------------------------- */
		DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), circle->get_color(), true);

    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(corpse.get())) {

    	switch (this->debug_type) {
    		case D_DEFAULT: { } break;
    		case D_QUADTREE: { } break;
    		case D_BOUNDINGS: {
    			ftn::Rectangle bounds = polygon->get_corpse_bounds();
    			DrawRectangle(bounds.pos.x, bounds.pos.y, bounds.size.x, bounds.size.y, false, sf::Color::Red, true);
    			DrawCircle(polygon->get_pos_x(), polygon->get_pos_y(), 5, sf::Color::Red, true);
    		} break;
    		case D_COLLISIONS: { } break;
    		case D_NORMALS: {
    			std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = polygon->get_sides();
    			for (int i = 0; i<sides.size(); i++) {
    				sf::Vector2f edge_center = (sides.at(i).first + sides.at(i).second)/2.0f;
    				sf::Vector2f edge_vector = edge_center + ftn::Normalize(ftn::Norme(sides.at(i).first, sides.at(i).second))*G_VECTOR_SIZE;
    				DrawCircle(edge_center.x, edge_center.y, 5, sf::Color::Red, true);
    				Renderer::DrawLine(edge_center.x, edge_center.y, edge_vector.x, edge_vector.y, sf::Color::Red);
    			
    				std::pair<sf::Vector2f, sf::Vector2f> last_edge = sides.at((i-1) % sides.size());
    				std::pair<sf::Vector2f, sf::Vector2f> current_edge = sides.at(i);
    				
					sf::Vector2f point_center = last_edge.second;
    				sf::Vector2f point_vector = point_center + ftn::Normalize(ftn::Norme(last_edge.first, last_edge.second) + ftn::Norme(current_edge.first, current_edge.second))*G_VECTOR_SIZE;
    				DrawCircle(point_center.x, point_center.y, 5, sf::Color::Red, true);
    				Renderer::DrawLine(point_center.x, point_center.y, point_vector.x, point_vector.y, sf::Color::Red);
    			}
    		} break;
    		case D_FORCES: { } break;
    		case D_PAIRS: { } break;
    	}
    	
    	/* -------------------------------------- Default Drawing -------------------------------------- */
    	DrawPolygon(polygon->get_points(), polygon->get_color(), true);

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

	DrawInputs();
	Interface();
	DrawTexts();
}

void Renderer::DrawInputs() {
	switch (this->debug_type) {
		case D_DEFAULT:{ } break;
		case D_QUADTREE: {
			std::vector<ftn::Rectangle> quadtrees = this->system.get_quadtree()->get_all_bounds();
			for (int i = 0; i < quadtrees.size(); i++) { DrawQuadtree(quadtrees.at(i)); }
		} break;
		case D_BOUNDINGS:{ } break;
		case D_COLLISIONS: { } break;
    	case D_NORMALS: { } break;
		case D_FORCES:{ } break;
		case D_PAIRS:{
			// for (int i = 0; i < system.get_pairs_size(); i++) { DrawPair(system.get_pair(i)); }
			std::vector<ftn::Rectangle> quadtrees = this->system.get_quadtree()->get_all_bounds();
			for (int i = 0; i < quadtrees.size(); i++) { DrawQuadtree(quadtrees.at(i)); }
			for (int i = 0; i < system.get_quad_pairs_size(); i++) { DrawPair(system.get_quad_pair(i)); }
		} break;
	}

	switch (this->select_type) {
		case S_DEFAULT:
			break;
		case S_SELECT_MULTIPLE: {
			sf::Vector2f temp_pos = this->selected_area.pos;
			sf::Vector2f temp_size = this->selected_area.size;
			DrawRectangle(temp_pos.x, temp_pos.y, temp_size.x, temp_size.y, false, sf::Color::White, true);
		} break;
		case S_LAUNCH_CORPSE: {
			sf::Vector2f pA = this->selected_area.pos;
			sf::Vector2f pB = this->selected_area.pos+this->selected_area.size;
			DrawLine(pA.x, pA.y, pB.x, pB.y, sf::Color::White);
		} break;
		case S_DRAG_CORPSE: { } break;
		case S_CREATE_CIRCLE: {
			sf::Vector2f temp_pos = this->selected_area.pos;
			float temp_size = ftn::Length(this->selected_area.size);
			if (temp_pos != sf::Vector2f()) { DrawCircle(temp_pos.x, temp_pos.y, temp_size, sf::Color::White, true); }
		} break;
		case S_CREATE_POLYGON: {
			if (this->selected_corpses_diff.size() != 0) { DrawPolygon(this->selected_corpses_diff, sf::Color::White, true); }
		} break;
	}

	// Outline the selected bodies
	for (int i=0; i<selected_corpses_cursor.size(); i++ ) {
		int cursor = selected_corpses_cursor.at(i);
		if (system.get_corpse(cursor)->get_removed()) { continue; } // Removed

	    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(system.get_corpse(cursor).get())) {
			DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), sf::Color::White, true); 
	    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(system.get_corpse(cursor).get())) {
	    	DrawPolygon(polygon->get_points(), sf::Color::White, true);
	    }
	}
}

void Renderer::Interface() {

	DrawText(ftn::to_string(debug_values[0]), 0, 0, 30, true, C_SUN);
	DrawText("[D] Debug: " + ftn::to_string(debug_values[1]), this->window.getSize().x - 150, 0, 24, true, C_SUN);

	DrawRectangle(0, this->window.getSize().y - 35, this->window.getSize().x, 35, true, C_BLACK);

	DrawText("mouse [ " + ftn::to_string(round(debug_values[2])) + " ; " + ftn::to_string(round(debug_values[3])) + " ]", 10, this->window.getSize().y - 30, 18, true, C_SUN);
	DrawText("[ " + ftn::to_string(round(debug_values[4])) + " ; " + ftn::to_string(round(debug_values[5])) + " ]", 180, this->window.getSize().y - 30, 18, true, C_SUN);
	DrawText("camera x" +  ftn::to_string(debug_values[6]), 380, this->window.getSize().y - 30, 18, true, C_SUN);
	DrawText("[ " + ftn::to_string(debug_values[7]) + " ; " + ftn::to_string(debug_values[8]) + " ]", 510, this->window.getSize().y - 30, 18, true, C_SUN);
	
	DrawText("[r][t] dt: " + ftn::to_string(debug_values[10]), this->window.getSize().x - 310, this->window.getSize().y - 30, 18, true, C_SUN);
	
	std::string string_paused = "[space] paused: false";
	if (debug_values[9]) { string_paused = "[space] paused: true"; }
	DrawText(string_paused, this->window.getSize().x - 180, this->window.getSize().y - 30, 18, true, C_SUN);

	std::string string_select = "";
	int select_value = debug_values[11];
	switch (select_value) {
		case S_DEFAULT: { string_select = "[Left/Right] Default selection"; } break;
		case S_SELECT_MULTIPLE: { string_select = "[Right] Multiple selection"; } break;
		case S_LAUNCH_CORPSE: { string_select = "[Right] Launch corpse"; } break;
		case S_DRAG_CORPSE: { string_select = "[Left] Drag selection"; } break;
		case S_DRAG_SCREEN: { string_select = "[Left] Drag screen"; } break;
		case S_CREATE_CIRCLE: { string_select = "[Left/Right] Create circle"; } break;
		case S_CREATE_POLYGON: { string_select = "[Left/Right] Create polygon"; } break;
	}
	DrawText(string_select, 43, 5, 20, true, C_SUN);
	DrawText(ftn::to_string(debug_values[12]) + " bodies", this->window.getSize().x - 470, this->window.getSize().y - 30, 18, true, C_SUN);
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

void Renderer::DrawCircle(int x, int y, int radius, sf::Color color, bool outline) {
	sf::CircleShape circle(radius, G_CIRCLE_RESOLUTION);

    // test if the circle is in the screen bounds
	circle.setPosition(x, y);
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	if (outline) {
		circle.setOutlineThickness(G_OUTLINE_THICKNESS);
		circle.setOutlineColor(color);
		circle.setFillColor(sf::Color::Transparent);
	} else {
		circle.setFillColor(color);
	}	
	
	if (((x + radius > get_real_pos_x(0)) && (x - radius < get_real_pos_x(screen_width)) && (y + radius > get_real_pos_y(0)) && (y - radius < get_real_pos_y(screen_height))) || ((x > get_real_pos_x(0)) && (x < get_real_pos_x(screen_width)) && (y > get_real_pos_y(0)) && (y < get_real_pos_y(screen_height)))) {
		this->window.draw(circle);
	}
}

void Renderer::DrawRectangle(int x, int y, int width, int height, bool fixed, sf::Color color, bool outline) {
	sf::RectangleShape rect(sf::Vector2f(width, height));
	if (fixed) {
		rect.setPosition(get_real_pos_x(x), get_real_pos_y(y));
		rect.scale(get_camera_size(), get_camera_size());
	} else {
    	rect.setPosition(x, y);
	}

	if (outline) {
		rect.setOutlineThickness(G_OUTLINE_THICKNESS);
		rect.setOutlineColor(color);
		rect.setFillColor(sf::Color::Transparent);
	} else {
		rect.setFillColor(color);
	}
	
	// To fix: check in screen for only non fixed pos
	if (rect_in_screen({sf::Vector2f(x, y), sf::Vector2f(width, height)})) {
		this->window.draw(rect);
	}
}

void Renderer::DrawPolygon(std::vector<sf::Vector2f> points, sf::Color color, bool outline) {
	sf::ConvexShape convex;

	convex.setPointCount(points.size());
	for (int i=0; i<points.size(); i++) { convex.setPoint(i, points.at(i)); }
	
	if (outline) {
		convex.setOutlineThickness(G_OUTLINE_THICKNESS);
		convex.setOutlineColor(color);
		convex.setFillColor(sf::Color::Transparent);
	} else {
		convex.setFillColor(color);
	}

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

