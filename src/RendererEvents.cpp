#include "../include/Renderer.hpp"

void Renderer::Input(sf::Event event) {
    ImGui::SFML::ProcessEvent(event);

    /* Manage the closure Event */
    if (event.type == sf::Event::Closed) {
        Close();
    }

    /* Update the view to the new size of the window */
    if (event.type == sf::Event::Resized) {
        set_screen_width(event.size.width);
        set_screen_height(event.size.height);
    }

    /* Check if the Inputs are Allowed */
    if (!this->get_enable_inputs()) {
        return;
    }

    /* Prevent events on ImGui from falling into SFML */
    ImGuiIO &io = ImGui::GetIO();

    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            if (io.WantCaptureMouse) break;

            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    /* If the mouse is not on an Corpse, Drag screen */
                    if (!SelectUniqueCorpseInit(event)) {
                        DragPositionInit(event);
                    }

                    CreatePolygonStop(event);
                    CreateCircleInit(event);
                } break;
                case sf::Mouse::Right: {
                    if (!LaunchCorpseInit(event)) {
                        SelectMultipleCorpsesInit(event);
                    }
                    CreateCircleFast(event);
                    CreatePolygonAddPoint(event);
                } break;
            }
        } break;

        case sf::Event::MouseMoved: {
            if (io.WantCaptureMouse) break;

            UpdateMouse();
            switch (this->select_type) {
                case S_DEFAULT: {
                } break;
                case S_SELECT_MULTIPLE: {
                    SelectMultipleCorpsesStep(event);
                } break;
                case S_LAUNCH_CORPSE: {
                    LaunchCorpseStep(event);
                } break;
                case S_DRAG_CORPSE: {
                    DragCorpsesStep(event);
                } break;
                case S_DRAG_SCREEN: {
                    DragPositionStep(event);
                } break;
                case S_CREATE_CIRCLE: {
                    CreateCircleStep(event);
                } break;
                case S_CREATE_POLYGON: {
                    CreatePolygonStep(event);
                } break;
            }
        } break;

        case sf::Event::MouseButtonReleased: {
            if (io.WantCaptureMouse) break;

            // Handle the functions associated with the mouse release
            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    DragPositionStop(event);
                    DragCorpsesStop(event);
                    // CreatePolygonStop(event);
                    CreateCircleStop(event);
                } break;
                case sf::Mouse::Right: {
                    LaunchCorpseStop(event);
                    SelectMultipleCorpsesStop(event);
                } break;
            }
        } break;

        case sf::Event::MouseWheelScrolled: {
            if (io.WantCaptureMouse) break;

            UpdateMouse();
            Camera(sf::Vector2f(0.0f, 0.0f), 1.0f - (event.mouseWheelScroll.delta * I_ZOOM_SPEED));
        } break;

        case sf::Event::KeyPressed: {
            if (io.WantCaptureKeyboard) break;

            // Handle the functions associated with the keyboard buttons
            switch (event.key.code) {
                case sf::Keyboard::D: {
                } break;
                case sf::Keyboard::R: {
                    this->system.add_dt(-100);
                } break;
                case sf::Keyboard::T: {
                    this->system.add_dt(100);
                } break;
                case sf::Keyboard::Space: {
                    Pause();
                } break;
                case sf::Keyboard::A: {
                    ToggleOnCircle(event);
                } break;
                case sf::Keyboard::Z: {
                    ToggleOnPolygon(event);
                } break;
            }
        } break;

        case sf::Event::KeyReleased: {
            if (io.WantCaptureKeyboard) break;

            switch (event.key.code) {
                case sf::Keyboard::A: {
                    ToggleOffCircle(event);
                } break;
                case sf::Keyboard::Z: {
                    ToggleOffPolygon(event);
                } break;
            }
        } break;
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

bool Renderer::DragPositionInit(sf::Event event) {
    if (this->select_type != S_DEFAULT) {
        return false;
    }

    this->select_type = S_DRAG_SCREEN;
    this->saved_mouse_pos = get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    return true;
}

void Renderer::DragPositionStep(sf::Event event) {
    if (this->select_type != S_DRAG_SCREEN) {
        return;
    }

    const sf::Vector2f new_pos = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
    this->Camera(this->saved_mouse_pos - new_pos);
}

void Renderer::DragPositionStop(sf::Event event) {
    if (this->select_type == S_DRAG_SCREEN) {
        this->select_type = S_DEFAULT;
    }
}

bool Renderer::SelectUniqueCorpseInit(sf::Event event) {
    if (this->select_type != S_DEFAULT) {
        return false;
    }

    // If already selected by multiple selection
    if (selected_corpses_cursor.size() > 0) {
        bool one_pointed = false;
        for (int i = 0; i < selected_corpses_cursor.size(); i++) {
            if (system.get_corpse(selected_corpses_cursor.at(i))->Pointed(this->sys_mouse_x, this->sys_mouse_y)) {
                one_pointed = true;
                break;
            }
        }

        if (one_pointed) {
            for (int i = 0; i < selected_corpses_cursor.size(); i++) {
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
        if (system.get_corpse(i)->get_removed()) {
            continue;
        }  // Removed
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
    // this->selected_corpses_diff = {};

    if (this->select_type != S_DEFAULT) {
        return;
    }
    this->selected_area = {get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), sf::Vector2f()};
    this->select_type = S_SELECT_MULTIPLE;
}

void Renderer::SelectMultipleCorpsesStep(sf::Event event) {
    if (this->select_type != S_SELECT_MULTIPLE) {
        return;
    }
    this->selected_area.size = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - this->selected_area.pos;
}

void Renderer::SelectMultipleCorpsesStop(sf::Event event) {
    if (this->select_type != S_SELECT_MULTIPLE) {
        return;
    }

    // Reorganize the points in a rectangle ABCD (top left point A / bottom
    // right point C)
    ftn::Rectangle rectangle = ftn::Reorder_Rectangle(this->selected_area);

    for (int i = 0; i < system.get_corpses_size(); i++) {
        if (system.get_corpse(i)->get_removed()) {
            continue;
        }  // Removed
        if (phy::Circle *circle = dynamic_cast<phy::Circle *>(system.get_corpse(i).get())) {
            if (!ftn::rect_out_bounds(circle->get_corpse_bounds(), rectangle)) {
                this->selected_corpses_cursor.push_back(i);
                this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
            }
        } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(system.get_corpse(i).get())) {
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
    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
        if (system.get_corpse(selected_corpses_cursor.at(i))->get_removed()) {
            continue;
        }  // Removed
        system.get_corpse(selected_corpses_cursor.at(i))->Move(get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) + selected_corpses_diff.at(i), false);
        system.CorpseStop(selected_corpses_cursor.at(i));
    }
}

void Renderer::DragCorpsesStop(sf::Event event) {
    if (this->select_type != S_DRAG_CORPSE) {
        return;
    }
    this->select_type = S_DEFAULT;

    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
        if (system.get_corpse(selected_corpses_cursor.at(i))->get_removed()) {
            continue;
        }  // Removed
        system.get_corpse(selected_corpses_cursor.at(i))->set_fixed(selected_corpses_fixed.at(i));
        system.CorpseStop(selected_corpses_cursor.at(i));
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

bool Renderer::LaunchCorpseInit(sf::Event event) {
    if (this->select_type != S_DEFAULT) {
        return false;
    }

    // If already selected by multiple selection
    if (selected_corpses_cursor.size() > 0) {
        bool one_pointed = false;
        for (int i = 0; i < selected_corpses_cursor.size(); i++) {
            if (system.get_corpse(selected_corpses_cursor.at(i))->Pointed(this->sys_mouse_x, this->sys_mouse_y)) {
                one_pointed = true;
                break;
            }
        }

        if (one_pointed) {
            for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                int index = selected_corpses_cursor.at(i);
                // this->selected_corpses_diff.push_back(system.get_corpse(index)->get_pos()
                // - get_real_pos(sf::Vector2i(event.mouseButton.x,
                // event.mouseButton.y)));
                this->selected_corpses_fixed.push_back(system.get_corpse(index)->get_fixed());
                system.get_corpse(index)->set_fixed(true);
            }
            this->selected_area = {get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), sf::Vector2f()};

            this->select_type = S_LAUNCH_CORPSE;
            return true;
        }

        /* Make sure that the arrays are empty */
        this->selected_corpses_cursor = {};
        this->selected_corpses_fixed = {};
        this->selected_corpses_diff = {};
    }

    for (int i = 0; i < system.get_corpses_size(); i++) {
        if (system.get_corpse(i)->get_removed()) {
            continue;
        }  // Removed
        if (system.get_corpse(i)->Pointed(this->sys_mouse_x, this->sys_mouse_y)) {
            this->selected_corpses_cursor.push_back(i);
            // this->selected_corpses_diff.push_back(system.get_corpse(i)->get_pos()
            // - get_real_pos(sf::Vector2i(event.mouseButton.x,
            // event.mouseButton.y)));
            this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
            system.get_corpse(i)->set_fixed(true);

            this->selected_area = {system.get_corpse(i)->get_pos(), sf::Vector2f()};

            this->select_type = S_LAUNCH_CORPSE;
            return true;
        }
    }
    return false;
}

void Renderer::LaunchCorpseStep(sf::Event event) {
    if (this->select_type != S_LAUNCH_CORPSE) {
        return;
    }
    this->selected_area.size = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - this->selected_area.pos;
}

void Renderer::LaunchCorpseStop(sf::Event event) {
    if (this->select_type != S_LAUNCH_CORPSE) {
        return;
    }
    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
        if (selected_corpses_fixed.at(i)) {
            continue;
        }
        sf::Vector2f diff_vector = this->selected_area.pos - (this->selected_area.pos + this->selected_area.size);
        sf::Vector2f launch_vector = ftn::Normalize(diff_vector) * ftn::Length(diff_vector) * I_LAUNCH_POWER;
        system.get_corpse(selected_corpses_cursor.at(i))->Move(launch_vector);
        system.get_corpse(selected_corpses_cursor.at(i))->set_fixed(selected_corpses_fixed.at(i));
    }

    this->select_type = S_DEFAULT;

    /* Make sure that the arrays are empty */
    // this->selected_corpses_cursor = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

void Renderer::ToggleOnCircle(sf::Event event) {
    if (this->select_type != S_DEFAULT) {
        return;
    }
    this->select_type = S_CREATE_CIRCLE;

    /* Make sure that the arrays are empty */
    this->selected_corpses_cursor = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

void Renderer::ToggleOffCircle(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) {
        return;
    }
    this->select_type = S_DEFAULT;

    /* Make sure that the arrays are empty */
    this->selected_corpses_cursor = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

void Renderer::CreateCircleFast(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) {
        return;
    }
    sf::Vector2f temp_pos = get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, 40, 40, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
}

void Renderer::CreateCircleInit(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) {
        return;
    }
    this->selected_area = {get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), sf::Vector2f()};
}
void Renderer::CreateCircleStep(sf::Event event) { this->selected_area.size = get_real_pos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - this->selected_area.pos; }
void Renderer::CreateCircleStop(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) {
        return;
    }
    sf::Vector2f temp_pos = this->selected_area.pos;
    float temp_size = ftn::Length(this->selected_area.size);
    float temp_mass = (temp_size * temp_size) * 0.1f;
    system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, temp_size, temp_mass, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
    this->selected_area = {sf::Vector2f(), sf::Vector2f()};
}

void Renderer::ToggleOnPolygon(sf::Event event) {
    if (this->select_type != S_DEFAULT) {
        return;
    }
    this->select_type = S_CREATE_POLYGON;

    /* Make sure that the arrays are empty */
    this->selected_corpses_diff = {};
}
void Renderer::ToggleOffPolygon(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) {
        return;
    }
    this->select_type = S_DEFAULT;

    if (this->selected_corpses_diff.size() > 2) {
        phy::Polygon temp_poly = phy::Polygon({}, 10, 1, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_NEPHRITIS);
        for (int i = 0; i < this->selected_corpses_diff.size(); i++) {
            temp_poly.add_point(this->selected_corpses_diff.at(i));
        }
        system.addCorpse(temp_poly);
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_diff = {};
}

void Renderer::CreatePolygonInit(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) {
        return;
    }
}
void Renderer::CreatePolygonAddPoint(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) {
        return;
    }
    sf::Vector2f temp_pos = get_real_pos(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    this->selected_corpses_diff.push_back(temp_pos);
}
void Renderer::CreatePolygonStep(sf::Event event) {}
void Renderer::CreatePolygonStop(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) {
        return;
    }

    if (this->selected_corpses_diff.size() > 2) {
        phy::Polygon temp_poly = phy::Polygon({}, 10, 1, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_NEPHRITIS);
        for (int i = 0; i < this->selected_corpses_diff.size(); i++) {
            temp_poly.add_point(this->selected_corpses_diff.at(i));
        }
        system.addCorpse(temp_poly);
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_diff = {};
}
