#include "../../include/Corpses/Circle.hpp"

namespace phy {

Circle::Circle(gmt::UnitI x, gmt::UnitI y, gmt::UnitI size, gmt::UnitI mass, gmt::UnitI damping, gmt::UnitI speed_x, gmt::UnitI speed_y, gmt::UnitI rotation, gmt::UnitI motor, gmt::VectorI propulsor, bool fixed, bool tied, bool etherial, sf::Color color)
    : Corpse(mass, damping, fixed, tied, etherial, color) {
    this->current_pos = gmt::VectorI(x, y);
    this->last_pos = gmt::VectorI(x - speed_x, y - speed_y);

    this->current_rotation = gmt::UnitI(0);
    this->last_rotation = rotation;

    this->size = size;
    this->motor = motor;
    this->propulsor = propulsor;
}
Circle& Circle::operator=(const Circle& rhs) {
    Corpse::operator=(rhs);
    this->size = rhs.get_size();
    return *this;
}
Circle::~Circle() {}

int Circle::get_class() const { return ID_CIRCLE; }

void Circle::Step() {
    if (this->fixed) {
        this->last_pos = this->current_pos;
    } else {
        gmt::VectorI diff_pos = this->current_pos - this->last_pos;
        this->last_pos = this->current_pos;
        this->current_pos = this->current_pos + diff_pos;
    }

    if (this->tied) {
        this->last_rotation = std::fmod(this->current_rotation, gmt::UnitI(360));
    } else {
        gmt::UnitI diff_rotation = std::fmod(this->current_rotation - this->last_rotation, gmt::UnitI(360));
        this->last_rotation = this->current_rotation;
        this->current_rotation = std::fmod(this->current_rotation + diff_rotation, gmt::UnitI(360));
    }
    // Add the motor rotation even if the object is tied
    if (!gmt::decimal_equals(motor, gmt::UnitI(0), gmt::UnitI(0.0001))) { this->current_rotation = this->current_rotation + motor; }
}
void Circle::Stop() {
    this->last_pos = this->current_pos;
    this->last_rotation = this->current_rotation;
}

void Circle::Move(gmt::UnitI x, gmt::UnitI y, bool relative) {
    if (relative) {
        this->current_pos = this->current_pos + gmt::VectorI(x, y);
    } else {
        this->current_pos = gmt::VectorI(x, y);
    }
}
void Circle::Move(gmt::VectorI move, bool relative) {
    if (relative) {
        this->current_pos = this->current_pos + move;
    } else {
        this->current_pos = move;
    }
}
bool Circle::inBounds(gmt::UnitI x1, gmt::UnitI x2, gmt::UnitI y1, gmt::UnitI y2) {
    return ((this->current_pos.x + this->size > x1) && (this->current_pos.x - this->size < x2) && (this->current_pos.y + this->size > y1) && (this->current_pos.y - this->size < y2)) ||
           ((this->current_pos.x > x1) && (this->current_pos.x < x2) && (this->current_pos.y > y1) && (this->current_pos.y < y2));
}

bool Circle::Pointed(gmt::UnitI x, gmt::UnitI y) { return (gmt::VectorI::Distance(this->get_pos(), gmt::VectorI(x, y)) <= this->size); }

void Circle::Collision(std::shared_ptr<Corpse> a) {
    if (Circle* circle = dynamic_cast<Circle*>(a.get())) {
        // Circle / Circle Collision
        gmt::UnitI distance = gmt::VectorI::Distance(this->get_pos(), circle->get_pos());
        gmt::UnitI overlap = (this->get_size() + circle->get_size() - distance);
        if (overlap < 0) { return; }

        gmt::UnitI x_diff = this->get_pos_x() - circle->get_pos_x();
        gmt::UnitI y_diff = this->get_pos_y() - circle->get_pos_y();

        gmt::VectorI vector_response = gmt::VectorI(x_diff / distance, y_diff / distance) * overlap;
        // vector_response=gmt::Pow(vector_response,10);
        Corpse::CollisionResponse(this, circle, vector_response);
    } else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {
        // Circle / Polygon collision

        std::vector<std::pair<gmt::VectorI, gmt::VectorI>> sides = polygon->get_sides();

        // Collide if the center of the circle is in the polygon
        if (polygon->Pointed(this->get_pos().x, this->get_pos().y)) {
            // Find the closest point on edges
            std::pair<gmt::VectorI, gmt::VectorI> closest_side = gmt::Closest_Edge(sides, this->get_pos());

            gmt::VectorI closest_projection = gmt::Segment_Projection(closest_side.first, closest_side.second, this->get_pos());
            gmt::VectorI vector_response = gmt::Normalize(gmt::Norme(closest_side.second, closest_side.first)) * (gmt::Length(this->get_pos(), closest_projection) + this->get_size());
            Corpse::CollisionResponse(polygon, this, vector_response);
            return;
        }

        // Collide if one side of the polygon intersect with the circle

        for (int i = 0; i < sides.size(); i++) {
            auto test_intersect = gmt::VectorI::LineCercleIntersect(sides.at(i).first, sides.at(i).second, this->get_pos(), this->get_size());

            // Don't collide with any edge
            if (test_intersect.first == 0) { continue; }

            if (test_intersect.first == 1) {
                // Collide at the middle of an edge
                gmt::VectorI vector_response = (this->get_pos() - test_intersect.second).Normalize() * (gmt::VectorI::Distance(this->get_pos(), test_intersect.second) - this->get_size());
                Corpse::CollisionResponse(polygon, this, vector_response);
                return;

            } else if (test_intersect.first == 2) {
                // Collide with the first point of the edge (current edge + last edge)
                int last_edge = (i - 1) % sides.size();
                gmt::VectorI normals_average = gmt::VectorI::Normal(sides.at(last_edge).first, sides.at(last_edge).second) + gmt::VectorI::Normal(sides.at(i).first, sides.at(i).second);
                gmt::VectorI vector_response = normals_average.Normalize() * (gmt::VectorI::Distance(this->get_pos(), sides.at(i).first) - this->get_size());
                Corpse::CollisionResponse(polygon, this, vector_response);
                return;

            } else if (test_intersect.first == 3) {
                // Collide with the second point of the edge (current edge + next edge)
                int next_edge = (i + 1) % sides.size();
                gmt::VectorI normals_average = gmt::VectorI::Normal(sides.at(i).first, sides.at(i).second) + gmt::VectorI::Normal(sides.at(next_edge).first, sides.at(next_edge).second);
                gmt::VectorI vector_response = normals_average.Normalize() * (gmt::VectorI::Distance(this->get_pos(), sides.at(i).second) - this->get_size());
                Corpse::CollisionResponse(polygon, this, vector_response);
                return;
            }
        }
    }
}

gmt::UnitI Circle::get_size() const { return this->size; }
gmt::BoundsI Circle::get_corpse_bounds() const { return gmt::BoundsI(this->get_pos_x() - this->get_size(), this->get_pos_y() - this->get_size(), this->get_pos_x() + this->get_size(), this->get_pos_y() + this->get_size()); }

}  // namespace phy