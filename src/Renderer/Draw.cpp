
#include "../../include/Renderer/Renderer.hpp"

void Renderer::SetupDraw() {
    SetupCirclesShader();
    SetupOutlinesShader();
}

void Renderer::SetupCirclesShader() {
    const std::string vertexShader =
        "void main() {"
        "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
        "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "gl_FrontColor = gl_Color;"
        "}";

    const std::string fragmentShader =
        "/* ============== CIRCLE ============= */"
        "vec4 circle(vec2 uv, vec2 pos, float rad, vec4 color) {"
        "   float dx = abs(pos.x - uv.x);"
        "   float dy = abs(pos.y - uv.y);"
        "   float dist = sqrt(dx*dx + dy*dy);"
        "   return vec4(color[0], color[1], color[2], dist < rad);"
        "}"
        "/* ============== MAIN ============= */"
        "void main(void) {"
        "   vec2 uv = gl_TexCoord[0].xy;"
        "   vec2 center = vec2(0.5, 0.5);"
        "   gl_FragColor = circle(uv, center, 0.5, gl_Color);"
        "}";

    if (!this->circles_shader.loadFromMemory(vertexShader, fragmentShader)) { throw std::runtime_error("Circles shaders couldn't be loaded..."); }
}

void Renderer::SetupOutlinesShader() {
    const std::string vertexShader =
        "void main() {"
        "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
        "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "gl_FrontColor = gl_Color;"
        "}";

    const std::string fragmentShader =
        "/* ============== CIRCLE ============= */"
        "vec4 circle(vec2 uv, vec2 pos, float rad, vec4 color) {"
        "   float dx = abs(pos.x - uv.x);"
        "   float dy = abs(pos.y - uv.y);"
        "   float dist = sqrt(dx*dx + dy*dy);"
        "   return vec4(color[0], color[1], color[2], dist > rad * (1 - " +
        gmt::to_string(OUTLINE_RES) +
        ") && dist < rad);"
        "}"
        "/* ============== MAIN ============= */"
        "void main(void) {"
        "   vec2 uv = gl_TexCoord[0].xy;"
        "   vec2 center = vec2(0.5, 0.5);"
        "   gl_FragColor = circle(uv, center, 0.5, vec4(1.0, 1.0, 1.0, 1.0));"
        "}";

    if (!this->outlines_shader.loadFromMemory(vertexShader, fragmentShader)) { throw std::runtime_error("Outlines shaders couldn't be loaded..."); }
}

void Renderer::Draw() {
    // Reset the buffers
    this->vertices_buffer = {};
    this->circles_buffer = {};
    this->outlines_buffer = {};

    // Reset the counts
    this->triangles = 0;
    this->rectangles = 0;
    this->lines = 0;
    this->arrows = 0;
    this->circles = 0;
    this->springs = 0;
    this->polygons = 0;

    // Fill the buffer with objects to be drawn
    for (int i = 0; i < system.get_corpses_size(); i++) { DrawCorpse(system.get_corpse(i), corpses_colors[system.get_corpse(i)->get_id()]); }
    for (int i = 0; i < system.get_constraints_size(); i++) { DrawConstraint(system.get_constraint(i), constraints_colors[system.get_constraint(i)->get_id()]); }
    if (system.get_enable_limits()) { DrawLimits(); }

    // Draw the buffers objects all at once
    this->window.draw(&this->vertices_buffer[0], this->vertices_buffer.size(), sf::Triangles);

    // Draw the circles with a shader
    this->window.draw(&this->circles_buffer[0], this->circles_buffer.size(), sf::Triangles, &this->circles_shader);

    // Draw the circles outlines with a shader
    this->window.draw(&this->outlines_buffer[0], this->outlines_buffer.size(), sf::Triangles, &this->outlines_shader);
}

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
        for (int i = 0; i < polygon_vertices.vertices.size(); i++) { polygon_points.push_back((*polygon_vertices.vertices[i]).CloneSF()); }
        // DrawPolygon(polygon_points, color, true);

        std::vector<gmt::VerticesI> triangles = polygon->get_polygons();
        for (int i = 0; i < triangles.size(); i++) {
            gmt::VerticesI triangle_vertices = triangles[i];
            DrawPolygon(triangle_vertices, color, false);
        }
        /* ---------------------------------------------------- Default Drawing ---------------------------------------------------- */

        if (debug_show_projections) {
            std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> tpairs = polygon_vertices.Pairs();
            for (int i = 0; i < tpairs.size(); i++) {
                gmt::VectorI tpoint = gmt::VectorI::SegmentProjection(gmt::VectorI(this->sys_mouse_x, this->sys_mouse_y), *tpairs[i].first, *tpairs[i].second);
                // DrawLine((*tpairs[i].first).x, (*tpairs[i].first).y, (*tpairs[i].second).x, (*tpairs[i].second).y, 1.0f, sf::Color::Yellow);
                DrawLine((*tpairs[i].first).x, (*tpairs[i].first).y, (*tpairs[i].second).x, (*tpairs[i].second).y, 5.0f, sf::Color(255, i * 255 / tpairs.size(), 0));
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
                gmt::VectorI point = *points.vertices[i];
                DrawCircle(point.x, point.y, 6, sf::Color::Red, true);
            }
        }
        if (debug_show_normals) {
            std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> sides = polygon->get_sides();
            for (int i = 0; i < sides.size(); i++) {
                sf::Vector2f side_A = (*sides[i].first).CloneSF();
                sf::Vector2f side_B = (*sides[i].second).CloneSF();

                sf::Vector2f edge_center = (side_A + side_B) / 2.0f;
                sf::Vector2f edge_vector = edge_center + (gmt::Vector<float>::Normal(side_A, side_B)).Normalize().CloneSF() * vector_size;
                Renderer::DrawArrow(edge_center.x, edge_center.y, edge_vector.x, edge_vector.y, arrow_size, arrow_size, line_thickness, sf::Color::Red);

                sf::Vector2f last_edge_A = (*sides[gmt::modulo(i - 1, sides.size())].first).CloneSF();
                sf::Vector2f last_edge_B = (*sides[gmt::modulo(i - 1, sides.size())].second).CloneSF();
                sf::Vector2f current_edge_A = (*sides[i].first).CloneSF();
                sf::Vector2f current_edge_B = (*sides[i].second).CloneSF();

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
        if (debug_show_edges) {
            std::vector<gmt::VerticesI> triangles = polygon->get_polygons();
            for (int i = 0; i < triangles.size(); i++) {
                gmt::VerticesI triangle_vertices = triangles[i];
                std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> sides = triangle_vertices.Pairs();
                for (int i = 0; i < sides.size(); i++) {
                    sf::Vector2f side_A = (*sides[i].first).CloneSF();
                    sf::Vector2f side_B = (*sides[i].second).CloneSF();
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
                std::pair<float, float> current = trajectories_previews[index][j];
                std::pair<float, float> next = trajectories_previews[index][j + 1];

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
        for (int i = 0; i < quadbounds.size(); i++) { DrawQuadTree(quadbounds[i]); }
    }

    if (debug_show_pairs) {
        for (int i = 0; i < system.get_corpses_size(); i++) {
            gmt::VectorI pos_A = system.get_corpse(i)->get_pos();
            for (int j = i + 1; j < system.get_corpses_size(); j++) {
                gmt::VectorI pos_B = system.get_corpse(j)->get_pos();
                DrawLine(pos_A.x, pos_A.y, pos_B.x, pos_B.y, line_thickness, sf::Color::White);
            }
        }
    }

    if (debug_show_quadpairs) {
        for (int j = 0; j < system.get_quad_pairs_depth(); j++) {
            for (int i = 0; i < system.get_quad_pairs_size(j); i++) {
                const std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>> pair = system.get_quad_pair(i, j);

                sf::Vector2f pos_A = (pair.first->get_pos()).CloneSF();
                sf::Vector2f pos_B = (pair.second->get_pos()).CloneSF();

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
            sf::Vector2f temp_pos = gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1).CloneSF();
            float temp_size = gmt::Vector<float>::Distance(gmt::Vector<float>(this->selected_area.x1, this->selected_area.y1), gmt::Vector<float>(this->selected_area.x2, this->selected_area.y2));

            if (temp_pos != sf::Vector2f()) { DrawCircle(temp_pos.x, temp_pos.y, temp_size, sf::Color::White, true); }
        } break;
        case S_CREATE_POLYGON: {
            if (this->selected_corpses_diff.size() != 0) { DrawPolygon(gmt::VerticesI(this->selected_corpses_diff), sf::Color::White, true); }
        } break;
    }

    // Outline the selected bodies
    for (int i = 0; i < selected_corpses_cursor.size(); i++) {
        int cursor = selected_corpses_cursor[i];

        if (phy::Circle *circle = dynamic_cast<phy::Circle *>(system.get_corpse(cursor).get())) {
            DrawCircle(circle->get_pos_x(), circle->get_pos_y(), circle->get_size(), sf::Color::White, true);
        } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(system.get_corpse(cursor).get())) {
            gmt::VerticesI polygon_vertices = polygon->get_points();
            DrawPolygon(polygon_vertices, sf::Color::White, true);
        }
    }
}