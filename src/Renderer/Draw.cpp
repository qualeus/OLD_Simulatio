
#include "../../include/Renderer/Renderer.hpp"

void Renderer::DrawCorpse(std::shared_ptr<phy::Corpse> corpse, sf::Color color) {
    if (phy::Circle *circle = dynamic_cast<phy::Circle *>(corpse.get())) {
        /* ---------------------------------------------------- Default Drawing ---------------------------------------------------- */
        DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), color, false);
        /* ---------------------------------------------------- Default Drawing ---------------------------------------------------- */

        if (debug_show_centroids) { DrawCircle(circle->get_pos_x(), circle->get_pos_y(), 5, sf::Color::Red, true); }
        if (debug_show_bounds) {
            gmt::BoundsI bounds = circle->get_bounds();
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

    } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(corpse.get())) {
        /* ---------------------------------------------------- Default Drawing ---------------------------------------------------- */
        gmt::VerticesI polygon_vertices = polygon->get_points();
        std::vector<sf::Vector2f> polygon_points = {};
        for (int i = 0; i < polygon_vertices.vertices.size(); i++) { polygon_points.push_back(convertSF(*polygon_vertices.vertices.at(i))); }
        // DrawPolygon(polygon_points, color, true);

        std::vector<gmt::VerticesI> triangles = polygon->get_polygons();
        for (int i = 0; i < triangles.size(); i++) {
            gmt::VerticesI triangle_vertices = triangles.at(i);
            std::vector<sf::Vector2f> triangle_points = {};
            for (int i = 0; i < triangle_vertices.vertices.size(); i++) { triangle_points.push_back(convertSF(*triangle_vertices.vertices.at(i))); }
            DrawPolygon(triangle_points, color, false);
        }
        /* ---------------------------------------------------- Default Drawing ---------------------------------------------------- */

        if (debug_show_projections) {
            std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> tpairs = polygon_vertices.Pairs();
            for (int i = 0; i < tpairs.size(); i++) {
                gmt::VectorI tpoint = gmt::VectorI::SegmentProjection(gmt::VectorI(this->sys_mouse_x, this->sys_mouse_y), *tpairs.at(i).first, *tpairs.at(i).second);
                // DrawLine((*tpairs.at(i).first).x, (*tpairs.at(i).first).y, (*tpairs.at(i).second).x, (*tpairs.at(i).second).y, 1.0f, sf::Color::Yellow);
                DrawLine((*tpairs.at(i).first).x, (*tpairs.at(i).first).y, (*tpairs.at(i).second).x, (*tpairs.at(i).second).y, 5.0f, sf::Color(255, i * 255 / tpairs.size(), 0));
                DrawCircle(tpoint.x, tpoint.y, 5, sf::Color(255, i * 255 / tpairs.size(), 0));
            }
        }
        if (debug_show_bounds) {
            gmt::BoundsI bounds = polygon->get_bounds();
            DrawRectangle(bounds.x1, bounds.y1, bounds.x2, bounds.y2, false, sf::Color::Red, true);
        }
        if (debug_show_centroids) { DrawCircle(polygon->get_pos_x(), polygon->get_pos_y(), 5, sf::Color::Red, true); }
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
                gmt::Vector<float> side_A = *sides.at(i).first;
                gmt::Vector<float> side_B = *sides.at(i).second;

                sf::Vector2f edge_center = (convertSF(side_A + side_B)) / 2.0f;
                sf::Vector2f edge_vector = edge_center + convertSF(gmt::Vector<float>::Normal(side_A, side_B).Normalize()) * vector_size;
                Renderer::DrawArrow(edge_center.x, edge_center.y, edge_vector.x, edge_vector.y, arrow_size, arrow_size, line_thickness, sf::Color::Red);

                gmt::Vector<float> last_edge_A = *sides.at(gmt::modulo(i - 1, sides.size())).first;
                gmt::Vector<float> last_edge_B = *sides.at(gmt::modulo(i - 1, sides.size())).second;
                gmt::Vector<float> current_edge_A = *sides.at(i).first;
                gmt::Vector<float> current_edge_B = *sides.at(i).second;

                sf::Vector2f point_vector = convertSF(last_edge_B) + convertSF(((gmt::Vector<float>::Normal(last_edge_A, last_edge_B)).Normalize() + (gmt::Vector<float>::Normal(current_edge_A, current_edge_B).Normalize())).Normalize()) * vector_size;
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
        if (debug_show_edges) {
            std::vector<gmt::VerticesI> triangles = polygon->get_polygons();
            for (int i = 0; i < triangles.size(); i++) {
                gmt::VerticesI triangle_vertices = triangles.at(i);
                std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> sides = triangle_vertices.Pairs();
                for (int i = 0; i < sides.size(); i++) {
                    sf::Vector2f side_A = convertSF(*sides.at(i).first);
                    sf::Vector2f side_B = convertSF(*sides.at(i).second);
                    DrawLine(side_A.x, side_A.y, side_B.x, side_B.y, 1.5f, sf::Color::Red);
                }
            }
        }
    }
}

void Renderer::DrawConstraint(std::shared_ptr<phy::Constraint> constraint, sf::Color color) {
    if (phy::Link *link = dynamic_cast<phy::Link *>(constraint.get())) {
        std::shared_ptr<phy::Corpse> corpse_a = link->get_corpse_a();
        std::shared_ptr<phy::Corpse> corpse_b = link->get_corpse_b();
        gmt::VectorI relative_pos_a = link->get_relative_pos_a().Rotate(link->get_relative_angle_a() - corpse_a->get_rotation());
        gmt::VectorI relative_pos_b = link->get_relative_pos_b().Rotate(link->get_relative_angle_b() - corpse_b->get_rotation());

        DrawLine(corpse_a->get_pos_x() + relative_pos_a.x, corpse_a->get_pos_y() + relative_pos_a.y, corpse_b->get_pos_x() + relative_pos_b.x, corpse_b->get_pos_y() + relative_pos_b.y, 5.0f, color);
    } else if (phy::Spring *spring = dynamic_cast<phy::Spring *>(constraint.get())) {
        int number_wave = static_cast<int>(spring->get_size() / spring->get_resolution());

        std::shared_ptr<phy::Corpse> corpse_a = spring->get_corpse_a();
        std::shared_ptr<phy::Corpse> corpse_b = spring->get_corpse_b();
        gmt::VectorI relative_pos_a = spring->get_relative_pos_a().Rotate(spring->get_relative_angle_a() - corpse_a->get_rotation());
        gmt::VectorI relative_pos_b = spring->get_relative_pos_b().Rotate(spring->get_relative_angle_b() - corpse_b->get_rotation());

        DrawSpring(corpse_a->get_pos_x() + relative_pos_a.x, corpse_a->get_pos_y() + relative_pos_a.y, corpse_b->get_pos_x() + relative_pos_b.x, corpse_b->get_pos_y() + relative_pos_b.y, spring->get_resolution(), number_wave, color);
    } else if (phy::Slider *slider = dynamic_cast<phy::Slider *>(constraint.get())) {
        std::shared_ptr<phy::Corpse> corpse_a = slider->get_corpse_a();
        std::shared_ptr<phy::Corpse> corpse_b = slider->get_corpse_b();
        gmt::VectorI relative_pos_a = slider->get_relative_pos_a().Rotate(slider->get_relative_angle_a() - corpse_a->get_rotation());
        gmt::VectorI relative_pos_b = slider->get_relative_pos_b().Rotate(slider->get_relative_angle_b() - corpse_b->get_rotation());

        DrawLine(corpse_a->get_pos_x() + relative_pos_a.x, corpse_a->get_pos_y() + relative_pos_a.y, corpse_b->get_pos_x() + relative_pos_b.x, corpse_b->get_pos_y() + relative_pos_b.y, 5.0f, color);
    }
}

void Renderer::DrawQuadTree(gmt::BoundsI rect) { DrawRectangle(rect.x1, rect.y1, rect.x2, rect.y2, false, C_CARROT, true); }

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

        // TODO Manage the corpses deletion:
        // Care also for teh reorder when the bodies
        // swap place with delete => use body id for
        // the array id (preview trajectories)

        // Initialize the vectors
        for (int j = 0; j < temp_system.get_corpses_size(); j++) {
            int index = temp_system.get_corpse(j)->get_id();
            trajectories_previews[index] = {};
            trajectories_previews[index].push_back({temp_system.get_corpse(j)->get_pos_x(), temp_system.get_corpse(j)->get_pos_y()});
        }

        for (int i = 0; i < trajectory_debug_step; i++) {
            for (int j = 0; j < trajectory_debug_time; j++) { temp_system.Step(); }

            for (int j = 0; j < temp_system.get_corpses_size(); j++) {
                int index = temp_system.get_corpse(j)->get_id();
                float pos_x = temp_system.get_corpse(j)->get_pos_x();
                float pos_y = temp_system.get_corpse(j)->get_pos_y();
                trajectories_previews[index].push_back({pos_x, pos_y});
            }
        }
    }

    // Draw the trajectories arrays
    for (auto &it : trajectories_previews) {
        int index = it.first;
        if (trajectory_debug_all || index == trajectory_debug_index) {
            if (trajectories_previews.size() < 1) { continue; }
            for (int j = 0; j < trajectories_previews[index].size() - 1; j++) {
                std::pair<float, float> current = trajectories_previews[index].at(j);
                std::pair<float, float> next = trajectories_previews[index].at(j + 1);

                float opacity = 255.0f - (255.0f * ((float)j / (float)trajectories_previews[index].size()));
                DrawLine(current.first, current.second, next.first, next.second, line_thickness, sf::Color(255, 255, 255, (int)opacity));
            }
        }
    }
}

void Renderer::Debug() {
    counter_debug++;

    this->collision_number += this->system.get_collisions_size() / this->system.get_collision_accuracy();
    if (counter_debug > DELAY_DEBUG) {
        UpdateDebug();
        counter_debug = 0;
        this->collision_number = 0;
    }

    DrawInputs();

    if (trajectory_debug_show && paused) { DrawTrajectories(); }
}

void Renderer::DrawInputs() {
    if (debug_show_quadtree) {
        std::vector<gmt::BoundsI> quadbounds = this->system.get_quadtree()->ComputeBounds();
        for (int i = 0; i < quadbounds.size(); i++) { DrawQuadTree(quadbounds.at(i)); }
    }

    if (debug_show_pairs) {
        for (int i = 0; i < system.get_pairs_size(); i++) {
            const std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair = system.get_pair(i);

            sf::Vector2f pos_A = convertSF(pair.first->get_pos());
            sf::Vector2f pos_B = convertSF(pair.second->get_pos());
            DrawLine(pos_A.x, pos_A.y, pos_B.x, pos_B.y);
        }
    }

    if (debug_show_quadpairs) {
        for (int j = 0; j < system.get_quad_pairs_depth(); j++) {
            for (int i = 0; i < system.get_quad_pairs_size(j); i++) {
                const std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair = system.get_quad_pair(i, j);

                sf::Vector2f pos_A = convertSF(pair.first->get_pos());
                sf::Vector2f pos_B = convertSF(pair.second->get_pos());

                DrawLine(pos_A.x, pos_A.y, pos_B.x, pos_B.y, 2.0f, sf::Color(255 - (225.0f / system.get_quad_pairs_depth() * j), 70, 0));
            }
        }
    }

    if (debug_show_contacts) {
        for (int i = 0; i < this->system.get_collisions_size(); i++) {
            gmt::CollisionI collision = this->system.get_collision(i);
            gmt::VectorI response = collision.normal.Normalize() * 10.0f;
            DrawLine(collision.origin.x - response.x, collision.origin.y - response.y, collision.origin.x + response.x, collision.origin.y + response.y, 3.0f, sf::Color::Red);
        }
    }

    if (debug_show_collisions) {
        for (int i = 0; i < this->system.get_collisions_size(); i++) {
            gmt::CollisionI collision = this->system.get_collision(i);
            console.Log(gmt::to_string(collision.origin) + " " + gmt::to_string(collision.normal));
            gmt::VectorI response = collision.normal * 2.0f;
            DrawLine(collision.origin.x - response.x, collision.origin.y - response.y, collision.origin.x + response.x, collision.origin.y + response.y, 3.0f, sf::Color::White);
        }
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
            sf::Vector2f temp_pos = convertSF(gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1));
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

        if (phy::Circle *circle = dynamic_cast<phy::Circle *>(system.get_corpse(cursor).get())) {
            DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), sf::Color::White, true);
        } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(system.get_corpse(cursor).get())) {
            gmt::VerticesI polygon_vertices = polygon->get_points();
            std::vector<sf::Vector2f> polygon_points = {};
            for (int i = 0; i < polygon_vertices.vertices.size(); i++) { polygon_points.push_back(convertSF(*polygon_vertices.vertices.at(i))); }
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

void Renderer::DrawSpring(int x1, int y1, int x2, int y2, float thickness, int number_wave, sf::Color color) {
    if (!gmt::Bounds<float>::SegmentIntersectBounds(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2), get_screen_bounds())) { return; }

    float inv = 0.25f / number_wave;
    float dx = (x2 - x1) * inv;
    float dy = (y2 - y1) * inv;

    float inv2 = thickness / std::sqrt(dx * dx + dy * dy);
    float px = dy * inv2;
    float py = -dx * inv2;

    float x = x1;
    float y = y1;
    for (int i = 0; i < number_wave; i++) {
        DrawLine(x, y, x + dx + px, y + dy + py, 2.0f, color);
        DrawLine(x + dx + px, y + dy + py, x + 3.0f * dx - px, y + 3.0f * dy - py, 2.0f, color);
        DrawLine(x + 3.0f * dx - px, y + 3.0f * dy - py, x + 4.0f * dx, y + 4.0f * dy, 2.0f, color);
        x += 4.0f * dx;
        y += 4.0f * dy;
    }
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
    if (!gmt::Bounds<float>::CircleIntersectBounds(radius, gmt::Vector<float>(x, y), get_screen_bounds())) { return; }

    sf::CircleShape circle(radius, circle_resolution);
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
    if (!gmt::Bounds<float>::BoundsIntersectBounds(gmt::Bounds<float>(x1, y1, x2, y2), get_screen_bounds())) { return; }

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
    bool pt_inbounds = false;
    for (int i = 0; i < points.size(); i++) {
        if (gmt::Bounds<float>::PointInBounds(gmt::Vector<float>(convertSF(points.at(i))), get_screen_bounds())) {
            pt_inbounds = true;
            break;
        }
    }
    if (!pt_inbounds) {
        return;  // If none of the point is in bounds return (TODO test segments intersection)
    }

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