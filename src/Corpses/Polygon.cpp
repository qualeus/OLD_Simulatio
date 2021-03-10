#include "../../include/Corpses/Polygon.hpp"

namespace phy {

Polygon::Polygon(std::initializer_list<gmt::VectorI> points, gmt::UnitI mass, gmt::UnitI damping, gmt::UnitI speed_x, gmt::UnitI speed_y, gmt::UnitI rotation, gmt::UnitI motor, gmt::VectorI propulsor, bool fixed, bool tied, bool etherial, sf::Color color)
    : Corpse(mass, damping, fixed, tied, etherial, color) {
    std::vector<gmt::VectorI> vect_points(std::begin(points), std::end(points));
    // this->points = vect_points;
    this->set_points(vect_points);

    /* Put all this in set_points in case of using it alone... */
    this->relative_points = init_relative_points(vect_points);

    gmt::VectorI computed_center = phy::Polygon::compute_center(vect_points);
    this->set_pos(computed_center);

    this->points_number = vect_points.size();
    this->last_pos = computed_center - gmt::VectorI(speed_x, speed_y);
    this->last_rotation = rotation;
    this->motor = motor;
    this->propulsor = propulsor;

    this->triangulate();
}
Polygon& Polygon::operator=(const Polygon& rhs) {
    Corpse::operator=(rhs);
    this->points_number = rhs.get_points_number();
    this->points = rhs.get_points();
    this->relative_points = rhs.get_relative_points();
    // TODO COPY Triangulation!!!
    // this->triangulation =
    // std::vector<std::vector<std::shared_ptr<gmt::VectorI>>> triangulation;
    return *this;
}
Polygon::~Polygon() {}

const int Polygon::get_class() { return ID_POLYGON; }
int Polygon::id_class() { return ID_POLYGON; }

void Polygon::Move(gmt::VectorI move) {
    this->current_pos = move;
    this->update_points();
}
void Polygon::Drag(gmt::VectorI drag) {
    this->current_pos = this->current_pos + drag;
    this->update_points();
}

bool Polygon::inBounds(gmt::UnitI x1, gmt::UnitI x2, gmt::UnitI y1, gmt::UnitI y2) { return true; }
bool Polygon::Pointed(gmt::UnitI x, gmt::UnitI y) {}

void Polygon::Collision(std::shared_ptr<Corpse> a) {
    if (Circle* circle = dynamic_cast<Circle*>(a.get())) {
        // Polygon / Circle collision
        std::vector<std::pair<gmt::VectorI, gmt::VectorI>> sides = this->get_sides();

        // Collide if the center of the circle is in the polygon
        if (this->Pointed(circle->get_pos().x, circle->get_pos().y)) {
            // Find the closest point on edges
            std::pair<gmt::VectorI, gmt::VectorI> closest_side = gmt::Closest_Edge(sides, circle->get_pos());

            gmt::VectorI closest_projection = gmt::Segment_Projection(closest_side.first, closest_side.second, circle->get_pos());
            gmt::VectorI vector_response = gmt::Normalize(gmt::Norme(closest_side.second, closest_side.first)) * (gmt::Length(circle->get_pos(), closest_projection) + circle->get_size());
            Corpse::CollisionResponse(this, circle, vector_response);
            return;
        }

        for (int i = 0; i < sides.size(); i++) {
            auto test_intersect = gmt::Line_Circle_Intersect(sides.at(i).first, sides.at(i).second, circle->get_pos(), circle->get_size());

            // Don't collide with any edge
            if (test_intersect.first == 0) { continue; }
            if (test_intersect.first == 1) {
                // Collide at the middle of an edge
                gmt::VectorI vector_response = gmt::Normalize(circle->get_pos() - test_intersect.second) * (gmt::Length(circle->get_pos(), test_intersect.second) - circle->get_size());
                Corpse::CollisionResponse(this, circle, vector_response);
                return;

            } else if (test_intersect.first == 2) {
                // Collide with the first point of the edge (current edge + last edge)
                int last_edge = (i - 1) % sides.size();
                gmt::VectorI normals_average = circle->get_pos() - sides.at(last_edge).second;
                gmt::VectorI vector_response = gmt::Normalize(normals_average) * (gmt::Length(circle->get_pos(), sides.at(i).first) - circle->get_size());
                Corpse::CollisionResponse(this, circle, vector_response);
                return;

            } else if (test_intersect.first == 3) {
                // Collide with the second point of the edge (current edge + next edge)
                int next_edge = (i + 1) % sides.size();
                gmt::VectorI normals_average = circle->get_pos() - sides.at(i).second;
                gmt::VectorI vector_response = gmt::Normalize(normals_average) * (gmt::Length(circle->get_pos(), sides.at(i).second) - circle->get_size());
                Corpse::CollisionResponse(this, circle, vector_response);
                return;
            }
        }

    } else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {
        // Polygon / Polygon collision (Separating axis theorem)

        // Make separating axis (perpendicular to the line that pass by the two objects center)
        gmt::VectorI axis = gmt::Norme(this->get_pos(), polygon->get_pos());  // gmt::VectorI axis = gmt::Norme(this->get_diff_pos(), polygon->get_diff_pos());

        // Find the two farthest points of the projection points on the separator axis
        std::vector<gmt::UnitI> self_projections = std::vector<gmt::UnitI>();
        std::vector<gmt::UnitI> other_projections = std::vector<gmt::UnitI>();

        for (int i = 0; i < this->get_points_number(); i++) { self_projections.push_back(gmt::Dot(this->get_points().at(i), axis)); }
        for (int i = 0; i < polygon->get_points_number(); i++) { other_projections.push_back(gmt::Dot(polygon->get_points().at(i), axis)); }

        const auto self_minmax = std::minmax_element(self_projections.begin(), self_projections.end());
        const auto other_minmax = std::minmax_element(other_projections.begin(), other_projections.end());

        gmt::UnitI self_min = *self_minmax.first;
        gmt::UnitI self_max = *self_minmax.second;
        gmt::UnitI other_min = *other_minmax.first;
        gmt::UnitI other_max = *other_minmax.second;
    }
}

void Polygon::update_points() {
    for (int i = 0; i < this->points_number; i++) { this->points.at(i) = this->get_pos() + this->relative_points.at(i); }
}

void Polygon::triangulate() {}

std::vector<gmt::VectorI> Polygon::init_relative_points(std::vector<gmt::VectorI> points) {
    gmt::VectorI pos = phy::Polygon::compute_center(points);
    this->set_pos(pos);
    std::vector<gmt::VectorI> relative_points = std::vector<gmt::VectorI>();
    for (int i = 0; i < points.size(); i++) { relative_points.push_back(points.at(i) - pos); }
    return relative_points;
}

void Polygon::set_points(gmt::VerticesI points) { this->points = points; }

void Polygon::add_point(gmt::VectorI point) {
    this->points.push_back(point);
    this->points_number++;

    gmt::VectorI diff_pos = this->current_pos - this->last_pos;
    gmt::VectorI computed_center = phy::Polygon::compute_center(this->points);

    this->set_pos(computed_center);
    this->last_pos = computed_center - diff_pos;
    this->relative_points = init_relative_points(this->points);
    this->update_points();
    this->triangulate();
}

void Polygon::remove_point(int i) {}

void Polygon::Step() {
    if (this->fixed) {
        this->last_pos = this->current_pos;
    } else {
        gmt::VectorI diff_pos = this->current_pos - this->last_pos;
        this->last_pos = this->current_pos;
        this->current_pos = this->current_pos + diff_pos;
    }

    if (this->tied) {
        this->last_rotation = std::fmod(this->current_rotation, 360);
    } else {
        gmt::UnitI diff_rotation = std::fmod(this->current_rotation - this->last_rotation, 360);
        this->last_rotation = this->current_rotation;
        this->current_rotation = std::fmod(this->current_rotation + diff_rotation, 360);
        for (int i = 0; i < this->relative_points.size(); i++) { gmt::Rotate(this->relative_points.at(i), current_rotation); }
    }

    if (!gmt::decimal_equals(motor, 0.0f, 0.0001f)) {
        // Add the motor rotation even if the object is tied
        this->current_rotation = this->current_rotation + motor;
        for (int i = 0; i < this->relative_points.size(); i++) { gmt::Rotate(this->relative_points.at(i), motor); }
    }

    this->update_points();
}
void Polygon::Stop() {
    this->last_pos = this->current_pos;
    this->last_rotation = this->current_rotation;
}

gmt::BoundsI Polygon::get_corpse_bounds() const { return this->points.Bounds(); }

gmt::VerticesI Polygon::get_points() const { return this->points; }
void Polygon::set_points(gmt::VerticesI points) { this->points = points; }

std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> Polygon::get_sides() const { return this->points.Pairs(); }

}  // namespace phy
