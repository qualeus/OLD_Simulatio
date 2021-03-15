
#include "../../include/Renderer/Renderer.hpp"

void Renderer::DrawCorpse(std::shared_ptr<phy::Corpse> corpse) {
    if (corpse->get_removed()) { return; }  // Removed

    if (phy::Circle *circle = dynamic_cast<phy::Circle *>(corpse.get())) {
        if (debug_show_centroids) { DrawCircle(circle->get_pos_x(), circle->get_pos_y(), 5, sf::Color::Red, true); }
        if (debug_show_rectangles) {
            gmt::BoundsI bounds = circle->get_corpse_bounds();
            DrawRectangle(bounds.x1, bounds.y1, bounds.x2, bounds.y2, false, sf::Color::Red, true);
        }
        if (debug_show_edges) { DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size() + 3, sf::Color::Red, true); }
        if (debug_show_vertices) {}
        if (debug_show_velocity) {
            DrawArrow(circle->get_pos_x(), circle->get_pos_y(), circle->get_pos_x() + (circle->get_diff_pos_x() / system.get_dt()) * velocity_size, circle->get_pos_y() + (circle->get_diff_pos_y() / system.get_dt()) * velocity_size, arrow_size, arrow_size, line_thickness, sf::Color::Red);
        }
        if (debug_show_xyvelocity) {
            DrawArrow(circle->get_pos_x(), circle->get_pos_y(), circle->get_pos_x() + (circle->get_diff_pos_x() / system.get_dt()) * velocity_size, circle->get_pos_y(), arrow_size, arrow_size, line_thickness, sf::Color::Blue);
            DrawArrow(circle->get_pos_x(), circle->get_pos_y(), circle->get_pos_x(), circle->get_pos_y() + (circle->get_diff_pos_y() / system.get_dt()) * velocity_size, arrow_size, arrow_size, line_thickness, sf::Color::Green);
        }

        /* -------------------------------------- Default Drawing -------------------------------------- */
        DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), circle->get_color(), true);

    } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(corpse.get())) {
        if (debug_show_rectangles) {
            gmt::BoundsI bounds = polygon->get_corpse_bounds();
            DrawRectangle(bounds.x1, bounds.y1, bounds.x2, bounds.y2, false, sf::Color::Red, true);
        }
        if (debug_show_centroids) { DrawCircle(polygon->get_pos_x(), polygon->get_pos_y(), 5, sf::Color::Red, true); }
        if (debug_show_edges) {
            std::vector<gmt::VerticesI> triangles = polygon->get_polygons();
            for (int i = 0; i < triangles.size(); i++) {
                gmt::VerticesI triangle_vertices = triangles.at(i);
                std::vector<sf::Vector2f> triangle_points = {};
                for (int i = 0; i < triangle_vertices.vertices.size(); i++) { triangle_points.push_back((*triangle_vertices.vertices.at(i)).CloneSF()); }
                DrawPolygon(triangle_points, sf::Color::Blue, false);
                DrawPolygon(triangle_points, sf::Color::Red, true);
            }
        }
        if (debug_show_vertices) {
            gmt::VerticesI points = polygon->get_points();
            for (int i = 0; i < points.vertices.size(); i++) {
                gmt::VectorI point = *points.vertices.at(i);
                DrawCircle(point.x, point.y, 6, sf::Color::Red, true);
            }
        }
        if (debug_show_normals) {
            std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> sides = polygon->get_sides();
            for (int i = 0; i < sides.size(); i++) {
                sf::Vector2f side_A = (*sides.at(i).first).CloneSF();
                sf::Vector2f side_B = (*sides.at(i).second).CloneSF();

                sf::Vector2f edge_center = (side_A + side_B) / 2.0f;
                sf::Vector2f edge_vector = edge_center + (gmt::Vector<float>::Normal(side_A, side_B)).Normalize().CloneSF() * vector_size;
                Renderer::DrawArrow(edge_center.x, edge_center.y, edge_vector.x, edge_vector.y, arrow_size, arrow_size, line_thickness, sf::Color::Red);

                sf::Vector2f last_edge_A = (*sides.at((i - 1) % sides.size()).first).CloneSF();
                sf::Vector2f last_edge_B = (*sides.at((i - 1) % sides.size()).second).CloneSF();
                sf::Vector2f current_edge_A = (*sides.at(i).first).CloneSF();
                sf::Vector2f current_edge_B = (*sides.at(i).second).CloneSF();

                sf::Vector2f point_vector = last_edge_B + ((gmt::Vector<float>::Normal(last_edge_A, last_edge_B)).Normalize() + (gmt::Vector<float>::Normal(current_edge_A, current_edge_B).Normalize())).Normalize().CloneSF() * vector_size;
                Renderer::DrawArrow(last_edge_B.x, last_edge_B.y, point_vector.x, point_vector.y, arrow_size, arrow_size, line_thickness, sf::Color::Red);
            }
        }
        if (debug_show_velocity) {
            DrawArrow(polygon->get_pos_x(), polygon->get_pos_y(), polygon->get_pos_x() + (polygon->get_diff_pos_x() / system.get_dt()) * velocity_size, polygon->get_pos_y() + (polygon->get_diff_pos_y() / system.get_dt()) * velocity_size, arrow_size, arrow_size, line_thickness, sf::Color::Red);
        }
        if (debug_show_xyvelocity) {
            DrawArrow(polygon->get_pos_x(), polygon->get_pos_y(), polygon->get_pos_x() + (polygon->get_diff_pos_x() / system.get_dt()) * velocity_size, polygon->get_pos_y(), arrow_size, arrow_size, line_thickness, sf::Color::Blue);
            DrawArrow(polygon->get_pos_x(), polygon->get_pos_y(), polygon->get_pos_x(), polygon->get_pos_y() + (polygon->get_diff_pos_y() / system.get_dt()) * velocity_size, arrow_size, arrow_size, line_thickness, sf::Color::Green);
        }

        /* -------------------------------------- Default Drawing -------------------------------------- */
        gmt::VerticesI polygon_vertices = polygon->get_points();
        std::vector<sf::Vector2f> polygon_points = {};
        for (int i = 0; i < polygon_vertices.vertices.size(); i++) { polygon_points.push_back((*polygon_vertices.vertices.at(i)).CloneSF()); }
        DrawPolygon(polygon_points, polygon->get_color(), true);
    }
}

void Renderer::DrawPair(std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair) {
    if (pair.first->get_removed() || pair.second->get_removed()) { return; }  // Removed

    sf::Vector2f pos_A = (pair.first->get_pos()).CloneSF();
    sf::Vector2f pos_B = (pair.second->get_pos()).CloneSF();

    DrawLine(pos_A.x, pos_A.y, pos_B.x, pos_B.y);
}

void Renderer::DrawQuadtree(gmt::BoundsI rect) { DrawRectangle(rect.x1, rect.y1, rect.x2, rect.y2, false, C_RED, true); }

void Renderer::DrawLimits() {
    gmt::BoundsI limits = system.get_limits();
    DrawRectangle(limits.x1, limits.y1, limits.x2, limits.y2, false, C_RED, true);
}

void Renderer::DrawTrajectories() {
    // If one body position have changed, recalculate all the trajectories
    if (debug_system_edited) {
        this->debug_system_edited = false;

        // Reset the trajectory array
        trajectories_previews = {};

        if (!trajectory_compute_on_change && !trajectory_compute_manual) { return; }
        this->trajectory_compute_manual = false;

        // Make a copy of the current system
        phy::System temp_system;
        temp_system = this->system;
        temp_system.set_collision_accuracy(trajectory_collision_accuracy);
        temp_system.set_constraint_accuracy(trajectory_constraint_accuracy);

        // Initialize the vectors
        for (int j = 0; j < temp_system.get_corpses_size(); j++) {
            trajectories_previews.push_back({});
            trajectories_previews.at(j).push_back({temp_system.get_corpse(j)->get_pos_x(), temp_system.get_corpse(j)->get_pos_y()});
        }

        for (int i = 0; i < trajectory_debug_step; i++) {
            for (int j = 0; j < trajectory_debug_time; j++) { temp_system.Step(); }

            for (int j = 0; j < temp_system.get_corpses_size(); j++) {
                float pos_x = temp_system.get_corpse(j)->get_pos_x();
                float pos_y = temp_system.get_corpse(j)->get_pos_y();
                trajectories_previews.at(j).push_back({pos_x, pos_y});
            }
        }
    }

    // Draw the trajectories arrays
    for (int i = 0; i < trajectories_previews.size(); i++) {
        if (trajectory_debug_all || i == trajectory_debug_index) {
            if (trajectories_previews.size() < 1) { continue; }

            for (int j = 0; j < trajectories_previews.at(i).size() - 1; j++) {
                std::pair<float, float> current = trajectories_previews.at(i).at(j);
                std::pair<float, float> next = trajectories_previews.at(i).at(j + 1);

                float opacity = 255.0f - (255.0f * ((float)j / (float)trajectories_previews.at(i).size()));
                DrawLine(current.first, current.second, next.first, next.second, line_thickness, sf::Color(255, 255, 255, (int)opacity));  // temp_corpses.at(i)->get_color());
            }
        }
    }
}

void Renderer::Debug() {
    counter_debug++;

    if (counter_debug > DELAY_DEBUG) {
        counter_debug = 0;
        UpdateDebug();
    }

    DrawInputs();
    DrawTexts();

    if (trajectory_debug_show && paused) { DrawTrajectories(); }
}

void Renderer::DrawInputs() {
    if (debug_show_quadtree) {
        std::vector<gmt::BoundsI> quadtrees = this->system.get_quadtree()->get_all_bounds();
        for (int i = 0; i < quadtrees.size(); i++) { DrawQuadtree(quadtrees.at(i)); }
    }

    if (debug_show_pairs) {
        for (int i = 0; i < system.get_quad_pairs_size(); i++) { DrawPair(system.get_quad_pair(i)); }
    }

    switch (this->select_type) {
        case S_DEFAULT: break;
        case S_SELECT_MULTIPLE: {
            DrawRectangle(this->selected_area.x1, this->selected_area.y1, this->selected_area.x2, this->selected_area.y2, false, sf::Color::White, true);
        } break;
        case S_LAUNCH_CORPSE: {
            DrawLine(this->selected_area.x1, this->selected_area.y1, this->selected_area.x2, this->selected_area.y2, line_thickness, sf::Color::White);
        } break;
        case S_DRAG_CORPSE: {
        } break;
        case S_CREATE_CIRCLE: {
            sf::Vector2f temp_pos = gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1).CloneSF();
            float temp_size = gmt::Vector<float>::Distance(gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1), gmt::Vector<float>(this->selected_area.x2, this->selected_area.y2));

            if (temp_pos != sf::Vector2f()) { DrawCircle(temp_pos.x, temp_pos.y, temp_size, sf::Color::White, true); }
        } break;
        case S_CREATE_POLYGON: {
            if (this->selected_corpses_diff.size() != 0) { DrawPolygon(this->selected_corpses_diff, sf::Color::White, true); }
        } break;
    }

    // Outline the selected bodies
    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
        int cursor = selected_corpses_cursor.at(i);
        if (system.get_corpse(cursor)->get_removed()) { continue; }  // Removed

        if (phy::Circle *circle = dynamic_cast<phy::Circle *>(system.get_corpse(cursor).get())) {
            DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), sf::Color::White, true);
        } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(system.get_corpse(cursor).get())) {
            gmt::VerticesI polygon_vertices = polygon->get_points();
            std::vector<sf::Vector2f> polygon_points = {};
            for (int i = 0; i < polygon_vertices.vertices.size(); i++) { polygon_points.push_back((*polygon_vertices.vertices.at(i)).CloneSF()); }
            DrawPolygon(polygon_points, sf::Color::White, true);
        }
    }
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2, float thickness, sf::Color color) {
    if (!gmt::Bounds<float>::SegmentIntersectBounds(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2), get_screen_bounds())) { return; }

    float length = gmt::Vector<float>::Distance(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2));
    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setOrigin(0, thickness / 2.0f);
    line.setPosition(x2, y2);
    line.rotate(gmt::Vector<float>::Bearing(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2)));
    line.setFillColor(color);
    this->window.draw(line);
}

void Renderer::DrawArrow(int x1, int y1, int x2, int y2, int xhead, int yhead, float thickness, sf::Color color) {
    if (!gmt::Bounds<float>::SegmentIntersectBounds(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2), get_screen_bounds())) { return; }

    float angle = gmt::Vector<float>::Bearing(gmt::Vector<float>(x2, y2), gmt::Vector<float>(x1, y1));
    float length = gmt::Vector<float>::Distance(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2));
    if (gmt::float_equals(length, 0.0f, min_arrow_size)) { return; }  // Dont draw if the vector is null

    sf::ConvexShape head = sf::ConvexShape(3);
    head.setPoint(0, {0.0f, 0.0f});
    head.setPoint(1, {(float)xhead, (float)yhead / 2.0f});
    head.setPoint(2, {0.0f, (float)yhead});
    head.setOrigin((float)xhead, (float)yhead / 2.0f);
    head.setPosition(x2, y2);
    head.setRotation(angle);
    head.setFillColor(color);

    const sf::Vector2f size = sf::Vector2f(length - (float)xhead, thickness);
    sf::RectangleShape tail = sf::RectangleShape(size);
    tail.setOrigin(0.0f, thickness / 2.0f);
    tail.setPosition(x1, y1);
    tail.setRotation(angle);
    tail.setFillColor(color);

    this->window.draw(head);
    this->window.draw(tail);
}

void Renderer::DrawCircle(int x, int y, int radius, sf::Color color, bool outline) {
    sf::CircleShape circle(radius, circle_resolution);

    // test if the circle is in the screen bounds
    circle.setPosition(x, y);
    circle.setOrigin(circle.getRadius(), circle.getRadius());

    if (outline) {
        circle.setOutlineThickness(outline_thickness);
        circle.setOutlineColor(color);
        circle.setFillColor(sf::Color::Transparent);
    } else {
        circle.setFillColor(color);
    }

    this->window.draw(circle);
}

void Renderer::DrawRectangle(int x1, int y1, int x2, int y2, bool fixed, sf::Color color, bool outline) {
    sf::RectangleShape rect(sf::Vector2f(x2 - x1, y2 - y1));
    if (fixed) {
        rect.setPosition(get_real_pos_x(x1), get_real_pos_y(y1));
        rect.scale(get_camera_size(), get_camera_size());
    } else {
        rect.setPosition(x1, y1);
    }

    if (outline) {
        rect.setOutlineThickness(outline_thickness);
        rect.setOutlineColor(color);
        rect.setFillColor(sf::Color::Transparent);
    } else {
        rect.setFillColor(color);
    }
    this->window.draw(rect);
}

void Renderer::DrawPolygon(std::vector<sf::Vector2f> points, sf::Color color, bool outline) {
    sf::ConvexShape convex;

    convex.setPointCount(points.size());
    for (int i = 0; i < points.size(); i++) { convex.setPoint(i, points.at(i)); }

    if (outline) {
        convex.setOutlineThickness(outline_thickness);
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

    if (font.loadFromFile("../assets/fonts/arial.ttf")) {
        text.setCharacterSize(text_resolution);
        float resolution_resize = size / text_resolution;

        if (fixed) {
            text.scale(get_camera_size() * resolution_resize, get_camera_size() * resolution_resize);
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
