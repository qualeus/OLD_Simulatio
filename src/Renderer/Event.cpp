
#include "../../include/Renderer/Renderer.hpp"

void Renderer::ClearSystem() {
    /* Make sure that the arrays are empty */
    this->selected_corpses_cursor = {};
    this->selected_corpses_index = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
    system.Clear();
}
void Renderer::UpdateSpawners() {
    int i = 0;
    while (i < this->spawners.size()) {
        if (this->spawners.at(i).interval_counter < this->spawners.at(i).interval) {
            this->spawners.at(i).interval_counter++;
        } else {
            StepSpawner(&this->spawners.at(i));
            this->spawners.at(i).duration--;
            this->spawners.at(i).interval_counter = 0;
        }

        if (this->spawners.at(i).duration != 0) {
            i++;
        } else {
            gmt::remove_unordered(i, this->spawners);
        }
    }
}

void Renderer::StepSpawner(Spawner *spawner) {
    std::vector<std::shared_ptr<phy::Corpse>> spawner_corpses = {};

    switch (spawner->corpse_type) {
        case 0: {
            for (int i = 0; i < spawner->corpse_number; i++) {
                spawner_corpses.push_back(
                    std::make_shared<phy::Circle>(phy::Circle(spawner->positionX, spawner->positionY, spawner->corpse_radiusX, spawner->corpse_mass, spawner->corpse_damping, 0.0f, 0.0f, 0.0f, 0.0f, spawner->corpse_fixed, spawner->corpse_tied, spawner->corpse_etherial,
                                                              {static_cast<unsigned char>(spawner->corpse_color[0]), static_cast<unsigned char>(spawner->corpse_color[1]), static_cast<unsigned char>(spawner->corpse_color[2]), static_cast<unsigned char>(spawner->corpse_color[3])})));
            }
        } break;
        case 1: {
            for (int i = 0; i < spawner->corpse_number; i++) {
                std::vector<gmt::VectorI> points = {};
                for (int j = 0; j < spawner->corpse_points_number; j++) {
                    float angle = gmt::degree_to_radian(static_cast<float>(j) * 360.0f / spawner->corpse_points_number);
                    if (gmt::float_equals(spawner->corpse_radius_randomX, 0.0f, 0.1f) && gmt::float_equals(spawner->corpse_radius_randomX, 0.0f, 0.1f)) {
                        points.push_back(gmt::VectorI(spawner->positionX + spawner->corpse_radiusX * std::cos(angle), spawner->positionY + spawner->corpse_radiusX * std::sin(angle)));
                    } else {
                        points.push_back(gmt::VectorI(spawner->positionX + (spawner->corpse_radiusX + gmt::rand_interval(static_cast<int>(spawner->corpse_radius_randomX))) * std::cos(angle),
                                                      spawner->positionY + (spawner->corpse_radiusX + gmt::rand_interval(static_cast<int>(spawner->corpse_radius_randomX))) * std::sin(angle)));
                    }
                }
                spawner_corpses.push_back(
                    std::make_shared<phy::Polygon>(phy::Polygon(points, spawner->corpse_mass, spawner->corpse_damping, 0.0f, 0.0f, 0.0f, 0.0f, spawner->corpse_fixed, spawner->corpse_tied, spawner->corpse_etherial,
                                                                {static_cast<unsigned char>(spawner->corpse_color[0]), static_cast<unsigned char>(spawner->corpse_color[1]), static_cast<unsigned char>(spawner->corpse_color[2]), static_cast<unsigned char>(spawner->corpse_color[3])})));
            }
        } break;
        case 2: {
            for (int i = 0; i < spawner->corpse_number; i++) {
                std::vector<gmt::VectorI> points = {};
                for (int j = 0; j < spawner->corpse_points_number; j++) {
                    float angle = gmt::degree_to_radian(static_cast<float>(j) * 360.0f / spawner->corpse_points_number);
                    if (gmt::float_equals(spawner->corpse_radius_randomX, 0.0f, 0.1f) && gmt::float_equals(spawner->corpse_radius_randomY, 0.0f, 0.1f)) {
                        points.push_back(gmt::VectorI(spawner->positionX + spawner->corpse_radiusX * std::cos(angle), spawner->positionY + spawner->corpse_radiusY * std::sin(angle)));
                    } else {
                        points.push_back(gmt::VectorI(spawner->positionX + (spawner->corpse_radiusX + gmt::rand_interval(static_cast<int>(spawner->corpse_radius_randomX))) * std::cos(angle),
                                                      spawner->positionY + (spawner->corpse_radiusY + gmt::rand_interval(static_cast<int>(spawner->corpse_radius_randomY))) * std::sin(angle)));
                    }
                }
                spawner_corpses.push_back(
                    std::make_shared<phy::Polygon>(phy::Polygon(points, spawner->corpse_mass, spawner->corpse_damping, 0.0f, 0.0f, 0.0f, 0.0f, spawner->corpse_fixed, spawner->corpse_tied, spawner->corpse_etherial,
                                                                {static_cast<unsigned char>(spawner->corpse_color[0]), static_cast<unsigned char>(spawner->corpse_color[1]), static_cast<unsigned char>(spawner->corpse_color[2]), static_cast<unsigned char>(spawner->corpse_color[3])})));
            }
        } break;
        default: {
        }
    }

    for (int i = 0; i < spawner->corpse_number; i++) {
        float random_rotation = 0.0f;
        float random_turn = 0.0f;
        if (!gmt::float_equals(spawner->launch_rotation_random, 0.0f, 0.1f)) { random_turn += gmt::rand_interval(static_cast<int>(spawner->launch_rotation_random)); }
        if (!gmt::float_equals(spawner->corpse_rotation_random, 0.0f, 0.1f)) { random_rotation += gmt::rand_interval(static_cast<int>(spawner->corpse_rotation_random)); }
        spawner_corpses.at(i)->Rotate(spawner->corpse_rotation + random_rotation);
        spawner_corpses.at(i)->Bloc();
        spawner_corpses.at(i)->Rotate(spawner->launch_rotation_power + random_turn);
    }

    switch (spawner->spawn_type) {
        case 0: {
            for (int i = 0; i < spawner->corpse_number; i++) {
                float randomX = 0.0f;
                float randomY = 0.0f;

                if (!gmt::float_equals(spawner->corpse_position_randomX, 0.0f, 0.1f)) { randomX += gmt::rand_interval_centered(static_cast<int>(spawner->corpse_position_randomX)); }
                if (!gmt::float_equals(spawner->corpse_position_randomY, 0.0f, 0.1f)) { randomY += gmt::rand_interval_centered(static_cast<int>(spawner->corpse_position_randomY)); }

                spawner_corpses.at(i)->Drag({randomX, randomY});
                spawner_corpses.at(i)->Stop();

                float angle = gmt::degree_to_radian(spawner->launch_direction);
                if (!gmt::float_equals(spawner->launch_direction_random, 0.0f, 0.1f)) { angle += gmt::rand_interval_centered(static_cast<int>(spawner->launch_direction_random)); }

                float power = spawner->launch_power;
                if (!gmt::float_equals(spawner->launch_random, 0.0f, 0.1f)) { power += gmt::rand_interval_centered(static_cast<int>(spawner->launch_random)); }

                spawner_corpses.at(i)->Drag({power * std::cos(angle), power * std::sin(angle)});
            }
        } break;
        case 1: {
            for (int i = 0; i < spawner->corpse_number; i++) {
                float angle = gmt::degree_to_radian(spawner->launch_direction);
                if (!gmt::float_equals(spawner->launch_direction_random, 0.0f, 0.1f)) { angle += gmt::rand_interval_centered(static_cast<int>(spawner->launch_direction_random)); }

                float power = spawner->launch_power;
                if (!gmt::float_equals(spawner->launch_random, 0.0f, 0.1f)) { power += gmt::rand_interval_centered(static_cast<int>(spawner->launch_random)); }

                spawner_corpses.at(i)->Drag({power * std::cos(angle), power * std::sin(angle)});
            }
        } break;
        case 2: {
            gmt::UnitI bounds_max_x = 0.0f;
            gmt::UnitI bounds_max_y = 0.0f;
            for (int i = 1; i < spawner->corpse_number; i++) {
                gmt::BoundsI corpse_bounds = spawner_corpses.at(i)->get_bounds();

                gmt::UnitI bounds_x = std::abs(corpse_bounds.x1 - corpse_bounds.x2);
                gmt::UnitI bounds_y = std::abs(corpse_bounds.y1 - corpse_bounds.y2);

                if (bounds_x > bounds_max_x) { bounds_max_x = bounds_x; }
                if (bounds_y > bounds_max_y) { bounds_max_y = bounds_y; }
            }

            gmt::UnitI semi_bounds_max_x = bounds_max_x / 2;
            gmt::UnitI semi_bounds_max_y = bounds_max_y / 2;

            spawner_corpses.at(0)->Move({spawner->positionX, spawner->positionY});
            spawner_corpses.at(0)->Stop();

            int corpse_index = 1;
            int ring_index = 0;  // index (6 lines)
            int ring_count = 1;  // width (line lenght)
            int line_count = 0;  // line

            gmt::VectorI last_pos = {spawner->positionX += bounds_max_x, spawner->positionY};

            while (corpse_index < spawner->corpse_number) {
                if (line_count >= ring_count) {
                    line_count = 0;
                    ring_index++;
                }
                if (ring_index > 5) {
                    ring_index = 0;
                    ring_count++;
                    last_pos.x += bounds_max_x;
                }

                gmt::VectorI current_pos = {last_pos.x, last_pos.y};
                switch (ring_index) {
                    case 0: {
                        current_pos = {current_pos.x - semi_bounds_max_x, current_pos.y - bounds_max_y};  // <\>
                    } break;
                    case 1: {
                        current_pos = {current_pos.x - bounds_max_x, current_pos.y};  // <->
                    } break;
                    case 2: {
                        current_pos = {current_pos.x - semi_bounds_max_x, current_pos.y + bounds_max_y};  // </>
                    } break;
                    case 3: {
                        current_pos = {current_pos.x + semi_bounds_max_x, current_pos.y + bounds_max_y};  // <\>
                    } break;
                    case 4: {
                        current_pos = {current_pos.x + bounds_max_x, current_pos.y};  // <->
                    } break;
                    case 5: {
                        current_pos = {current_pos.x + semi_bounds_max_x, current_pos.y - bounds_max_y};  // </>
                    } break;
                }

                spawner_corpses.at(corpse_index)->Move(current_pos);  // TODO diff between bounds pos and real pos (poly)
                spawner_corpses.at(corpse_index)->Stop();

                line_count++;
                corpse_index++;
                last_pos = current_pos;
            }

            for (int i = 0; i < spawner->corpse_number; i++) {
                float randomX = 0.0f;
                float randomY = 0.0f;

                if (!gmt::float_equals(spawner->corpse_position_randomX, 0.0f, 0.1f)) { randomX += gmt::rand_interval_centered(static_cast<int>(spawner->corpse_position_randomX)); }
                if (!gmt::float_equals(spawner->corpse_position_randomY, 0.0f, 0.1f)) { randomY += gmt::rand_interval_centered(static_cast<int>(spawner->corpse_position_randomY)); }

                spawner_corpses.at(i)->Drag({randomX, randomY});
                spawner_corpses.at(i)->Stop();

                float angle = gmt::degree_to_radian(spawner->launch_direction);
                if (!gmt::float_equals(spawner->launch_direction_random, 0.0f, 0.1f)) { angle += gmt::rand_interval_centered(static_cast<int>(spawner->launch_direction_random)); }

                float power = spawner->launch_power;
                if (!gmt::float_equals(spawner->launch_random, 0.0f, 0.1f)) { power += gmt::rand_interval_centered(static_cast<int>(spawner->launch_random)); }

                spawner_corpses.at(i)->Drag({power * std::cos(angle), power * std::sin(angle)});
            }
        } break;
        default: {
        }
    }

    for (int i = 0; i < spawner->corpse_number; i++) { system.add_corpse(spawner_corpses.at(i)); }
}

void Renderer::Input(sf::Event event) {
    ImGui::SFML::ProcessEvent(event);

    /* Manage the closure Event */
    if (event.type == sf::Event::Closed) { Close(); }

    /* Update the view to the new size of the window */
    if (event.type == sf::Event::Resized) {
        set_screen_width(event.size.width);
        set_screen_height(event.size.height);
    }

    /* Check if the Inputs are Allowed */
    if (!this->get_enable_inputs()) { return; }

    /* Prevent events on ImGui from falling into SFML */
    ImGuiIO &io = ImGui::GetIO();

    UpdateMouse();
    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            if (io.WantCaptureMouse) break;

            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    bool select_unique = SelectUniqueCorpseInit(event);

                    /* If the mouse is not on an Corpse, Drag 0screen */
                    if (!select_unique) { DragPositionInit(event); }
                    DragPositionInit(event);
                    CreatePolygonStop(event);
                    CreateCircleInit(event);
                } break;
                case sf::Mouse::Right: {
                    bool launch_corpse = LaunchCorpseInit(event);

                    /* If the mouse is not on an Corpse, Select area */
                    if (!launch_corpse) { SelectMultipleCorpsesInit(event); }

                    CreateCircleFast(event);
                    CreatePolygonAddPoint(event);
                } break;
            }
        } break;

        case sf::Event::MouseMoved: {
            if (io.WantCaptureMouse) break;

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

            Camera(sf::Vector2f(0.0f, 0.0f), 1.0f - (event.mouseWheelScroll.delta * zoom_speed));
        } break;

        case sf::Event::KeyPressed: {
            if (io.WantCaptureKeyboard) break;

            // Handle the functions associated with the keyboard buttons
            switch (event.key.code) {
                case sf::Keyboard::D: {
                } break;
                case sf::Keyboard::R: {
                    this->debug_system_edited = true;
                    this->system.set_dt(this->system.get_dt() - dt_step);
                } break;
                case sf::Keyboard::T: {
                    this->debug_system_edited = true;
                    this->system.set_dt(this->system.get_dt() + dt_step);
                } break;
                case sf::Keyboard::Space: {
                    this->debug_system_edited = true;
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
    this->sys_mouse_x = get_real_pos_x(mouse_x);
    this->sys_mouse_y = get_real_pos_y(mouse_y);
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
    this->Camera(this->saved_mouse_pos - new_pos);
}

void Renderer::DragPositionStop(sf::Event event) {
    if (this->select_type == S_DRAG_SCREEN) { this->select_type = S_DEFAULT; }
}

bool Renderer::SelectUniqueCorpseInit(sf::Event event) {
    if (this->select_type != S_DEFAULT) { return false; }

    // If already selected by multiple selection
    if (selected_corpses_cursor.size() > 0) {
        bool one_pointed = false;
        for (int i = 0; i < selected_corpses_cursor.size(); i++) {
            if (!gmt::BoundsI::BoundsIntersectBounds(system.get_corpse(i)->get_bounds(), gmt::BoundsI(get_real_pos_x(0), get_real_pos_y(0), get_real_pos_x(screen_width), get_real_pos_y(screen_height)))) { continue; }  // Out Screen

            if (system.get_corpse(selected_corpses_cursor.at(i))->Pointed(gmt::VectorI(this->sys_mouse_x, this->sys_mouse_y))) {
                one_pointed = true;
                break;
            }
        }

        if (one_pointed) {
            for (int i = 0; i < selected_corpses_cursor.size(); i++) {
                int index = selected_corpses_cursor.at(i);
                this->selected_corpses_diff.push_back(system.get_corpse(index)->get_pos().CloneSF() - sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y));
                this->selected_corpses_fixed.push_back(system.get_corpse(index)->get_fixed());
                system.get_corpse(index)->set_fixed(true);
            }
            this->select_type = S_DRAG_CORPSE;
            this->debug_system_edited = true;
            return true;
        }

        /* Make sure that the arrays are empty */
        this->selected_corpses_cursor = {};
        this->selected_corpses_index = {};
        this->selected_corpses_fixed = {};
        this->selected_corpses_diff = {};
    }

    /* If Selection is currently Empty */
    for (int i = 0; i < system.get_corpses_size(); i++) {
        if (!gmt::BoundsI::BoundsIntersectBounds(system.get_corpse(i)->get_bounds(), gmt::BoundsI(get_real_pos_x(0), get_real_pos_y(0), get_real_pos_x(screen_width), get_real_pos_y(screen_height)))) { continue; }  // Out Screen

        if (system.get_corpse(i)->Pointed(gmt::VectorI(this->sys_mouse_x, this->sys_mouse_y))) {
            this->selected_corpses_cursor.push_back(i);
            this->selected_corpses_index.push_back(system.get_corpse(i)->get_id());
            this->selected_corpses_diff.push_back(system.get_corpse(i)->get_pos().CloneSF() - sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y));
            // Fix the corpse while holding it
            this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
            system.get_corpse(i)->set_fixed(true);

            this->select_type = S_DRAG_CORPSE;
            this->debug_system_edited = true;
            return true;
        }
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_cursor = {};
    this->selected_corpses_index = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
    return false;
}

void Renderer::SelectMultipleCorpsesInit(sf::Event event) {
    /* Make sure that the arrays are empty */
    this->selected_corpses_cursor = {};
    this->selected_corpses_index = {};
    this->selected_corpses_fixed = {};
    // this->selected_corpses_diff = {};

    if (this->select_type != S_DEFAULT) { return; }

    sf::Vector2f mouse_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
    this->selected_area = gmt::Bounds<float>(mouse_pos.x, mouse_pos.y, mouse_pos.x, mouse_pos.y);

    this->select_type = S_SELECT_MULTIPLE;
}

void Renderer::SelectMultipleCorpsesStep(sf::Event event) {
    if (this->select_type != S_SELECT_MULTIPLE) { return; }
    sf::Vector2f mouse_real_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
    this->selected_area.x2 = mouse_real_pos.x;
    this->selected_area.y2 = mouse_real_pos.y;
}

void Renderer::SelectMultipleCorpsesStop(sf::Event event) {
    if (this->select_type != S_SELECT_MULTIPLE) { return; }

    // Reorganize the points in a rectangle ABCD (top left point A / bottom right point C)
    gmt::Bounds<float> s_rec = this->selected_area.Reorder();
    gmt::BoundsI rectangle = gmt::BoundsI(s_rec.x1, s_rec.y1, s_rec.x2, s_rec.y2);

    for (int i = 0; i < system.get_corpses_size(); i++) {
        if (phy::Circle *circle = dynamic_cast<phy::Circle *>(system.get_corpse(i).get())) {
            if (!gmt::BoundsI::BoundsOutBounds(circle->get_bounds(), rectangle)) {
                this->selected_corpses_cursor.push_back(i);
                this->selected_corpses_index.push_back(system.get_corpse(i)->get_id());
                this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
            }
        } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(system.get_corpse(i).get())) {
            if (!gmt::BoundsI::BoundsOutBounds(polygon->get_bounds(), rectangle)) {
                this->selected_corpses_cursor.push_back(i);
                this->selected_corpses_index.push_back(system.get_corpse(i)->get_id());
                this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
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
        this->debug_system_edited = true;
        system.get_corpse(selected_corpses_cursor.at(i))->Move(sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y) + selected_corpses_diff.at(i));
        system.CorpseStop(selected_corpses_cursor.at(i));
        system.CorpseUpdateBounds(selected_corpses_cursor.at(i));
    }
}

void Renderer::DragCorpsesStop(sf::Event event) {
    if (this->select_type != S_DRAG_CORPSE) { return; }
    this->debug_system_edited = true;
    this->select_type = S_DEFAULT;

    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
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
        for (int i = 0; i < selected_corpses_cursor.size(); i++) {
            if (!gmt::BoundsI::BoundsIntersectBounds(system.get_corpse(i)->get_bounds(), gmt::BoundsI(get_real_pos_x(0), get_real_pos_y(0), get_real_pos_x(screen_width), get_real_pos_y(screen_height)))) { continue; }  // Out Screen

            if (system.get_corpse(selected_corpses_cursor.at(i))->Pointed(gmt::VectorI(this->sys_mouse_x, this->sys_mouse_y))) {
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

            sf::Vector2f mouse_real_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
            this->selected_area = gmt::Bounds<float>(mouse_real_pos.x, mouse_real_pos.y, mouse_real_pos.x, mouse_real_pos.y);

            this->select_type = S_LAUNCH_CORPSE;
            this->debug_system_edited = true;
            return true;
        }

        /* Make sure that the arrays are empty */
        this->selected_corpses_cursor = {};
        this->selected_corpses_index = {};
        this->selected_corpses_fixed = {};
        this->selected_corpses_diff = {};
    }

    for (int i = 0; i < system.get_corpses_size(); i++) {
        if (!gmt::BoundsI::BoundsIntersectBounds(system.get_corpse(i)->get_bounds(), gmt::BoundsI(get_real_pos_x(0), get_real_pos_y(0), get_real_pos_x(screen_width), get_real_pos_y(screen_height)))) { continue; }  // Out Screen

        if (system.get_corpse(i)->Pointed(gmt::VectorI(this->sys_mouse_x, this->sys_mouse_y))) {
            this->selected_corpses_cursor.push_back(i);
            // this->selected_corpses_diff.push_back(system.get_corpse(i)->get_pos()
            // - get_real_pos(sf::Vector2i(event.mouseButton.x,
            // event.mouseButton.y)));
            this->selected_corpses_fixed.push_back(system.get_corpse(i)->get_fixed());
            system.get_corpse(i)->set_fixed(true);

            sf::Vector2f corpse_pos = system.get_corpse(i)->get_pos().CloneSF();
            this->selected_area = gmt::Bounds<float>(corpse_pos.x, corpse_pos.y, corpse_pos.x, corpse_pos.y);

            this->select_type = S_LAUNCH_CORPSE;
            this->debug_system_edited = true;
            return true;
        }
    }
    return false;
}

void Renderer::LaunchCorpseStep(sf::Event event) {
    if (this->select_type != S_LAUNCH_CORPSE) { return; }
    sf::Vector2f mouse_real_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
    this->selected_area.x2 = mouse_real_pos.x;
    this->selected_area.y2 = mouse_real_pos.y;
}

void Renderer::LaunchCorpseStop(sf::Event event) {
    if (this->select_type != S_LAUNCH_CORPSE) { return; }
    this->debug_system_edited = true;

    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
        if (selected_corpses_fixed.at(i)) { continue; }
        sf::Vector2f diff_vector = sf::Vector2f(this->selected_area.x2 - this->selected_area.x1, this->selected_area.y2 - this->selected_area.y1);
        sf::Vector2f launch_vector = -diff_vector * launch_power;
        system.get_corpse(selected_corpses_cursor.at(i))->Drag(launch_vector);
        system.get_corpse(selected_corpses_cursor.at(i))->set_fixed(selected_corpses_fixed.at(i));
        system.CorpseUpdateBounds(selected_corpses_cursor.at(i));
    }

    this->select_type = S_DEFAULT;

    /* Make sure that the arrays are empty */
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

void Renderer::ToggleOnCircle(sf::Event event) {
    if (this->select_type != S_DEFAULT) { return; }
    this->select_type = S_CREATE_CIRCLE;

    /* Make sure that the arrays are empty */
    this->selected_area = gmt::Bounds<float>();
    this->selected_corpses_cursor = {};
    this->selected_corpses_index = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

void Renderer::ToggleOffCircle(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) { return; }
    this->select_type = S_DEFAULT;
    if (this->selected_area != gmt::Bounds<float>()) {
        sf::Vector2f temp_pos = sf::Vector2f(this->selected_area.x1, this->selected_area.y1);
        float temp_size = gmt::Vector<float>::Distance(gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1), gmt::Vector<float>(this->selected_area.x2, this->selected_area.y2));
        float temp_mass = (temp_size * temp_size) * 0.1f;

        system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, temp_size, temp_mass, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
        this->selected_area = gmt::Bounds<float>();
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_cursor = {};
    this->selected_corpses_index = {};
    this->selected_corpses_fixed = {};
    this->selected_corpses_diff = {};
}

void Renderer::CreateCircleFast(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) { return; }
    this->debug_system_edited = true;

    sf::Vector2f temp_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
    system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, 40, 40, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
}

void Renderer::CreateCircleInit(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) { return; }

    sf::Vector2f mouse_real_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
    this->selected_area = gmt::Bounds<float>(mouse_real_pos.x, mouse_real_pos.y, mouse_real_pos.x, mouse_real_pos.y);
}
void Renderer::CreateCircleStep(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) { return; }
    if (this->selected_area.x1 != 0.0f || this->selected_area.y1 != 0.0f) {
        sf::Vector2f mouse_real_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
        this->selected_area.x2 = mouse_real_pos.x;
        this->selected_area.y2 = mouse_real_pos.y;
    }
}

void Renderer::CreateCircleStop(sf::Event event) {
    if (this->select_type != S_CREATE_CIRCLE) { return; }
    this->debug_system_edited = true;

    sf::Vector2f temp_pos = sf::Vector2f(this->selected_area.x1, this->selected_area.y1);
    float temp_size = gmt::Vector<float>::Distance(gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1), gmt::Vector<float>(this->selected_area.x2, this->selected_area.y2));
    float temp_mass = (temp_size * temp_size) * 0.1f;

    system.addCorpse(phy::Circle(temp_pos.x, temp_pos.y, temp_size, temp_mass, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
    this->selected_area = gmt::Bounds<float>();
}

void Renderer::ToggleOnPolygon(sf::Event event) {
    if (this->select_type != S_DEFAULT) { return; }
    this->select_type = S_CREATE_POLYGON;

    /* Make sure that the arrays are empty */
    this->selected_corpses_diff = {};
}
void Renderer::ToggleOffPolygon(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) { return; }
    this->debug_system_edited = true;

    this->select_type = S_DEFAULT;

    if (this->selected_corpses_diff.size() > 2) {
        std::vector<gmt::VectorI> points = {};
        for (int i = 0; i < this->selected_corpses_diff.size(); i++) { points.push_back(gmt::VectorI(this->selected_corpses_diff.at(i))); }
        phy::Polygon temp_poly = phy::Polygon(points, 10, 1, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_NEPHRITIS);
        system.addCorpse(temp_poly);
    } else {
        this->input_spawner.positionX = this->sys_mouse_x;
        this->input_spawner.positionY = this->sys_mouse_y;
        this->spawners.push_back(this->input_spawner);
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_diff = {};
}

void Renderer::CreatePolygonInit(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) { return; }
}
void Renderer::CreatePolygonAddPoint(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) { return; }
    sf::Vector2f temp_pos = sf::Vector2f(this->sys_mouse_x, this->sys_mouse_y);
    this->selected_corpses_diff.push_back(temp_pos);
}
void Renderer::CreatePolygonStep(sf::Event event) {}
void Renderer::CreatePolygonStop(sf::Event event) {
    if (this->select_type != S_CREATE_POLYGON) { return; }
    this->debug_system_edited = true;

    if (this->selected_corpses_diff.size() > 2) {
        std::vector<gmt::VectorI> points = {};
        for (int i = 0; i < this->selected_corpses_diff.size(); i++) { points.push_back(gmt::VectorI(this->selected_corpses_diff.at(i))); }
        phy::Polygon temp_poly = phy::Polygon(points, 10, 1, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_NEPHRITIS);
        system.addCorpse(temp_poly);
    }

    /* Make sure that the arrays are empty */
    this->selected_corpses_diff = {};
}
