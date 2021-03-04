#include "../include/Polygon.hpp"

namespace phy {

Polygon::Polygon(std::initializer_list<sf::Vector2f> points, float mass, float damping, float speed_x, float speed_y, float rotation, float motor, bool fixed, bool tied, bool etherial, sf::Color color) : Corpse(0.0f, 0.0f, mass, damping, fixed, tied, etherial, color) {
    std::vector<sf::Vector2f> vect_points(std::begin(points), std::end(points));
    // this->points = vect_points;
    this->set_points(vect_points);

    /* Put all this in set_points in case of using it alone... */
    this->relative_points = init_relative_points(vect_points);

    sf::Vector2f computed_center = phy::Polygon::compute_center(vect_points);
    this->set_pos(computed_center);

    this->points_number = vect_points.size();
    this->last_pos = computed_center - sf::Vector2f(speed_x, speed_y);
    this->last_rotation = rotation;
    this->motor_rotation = motor;

    this->triangulate();
}
Polygon& Polygon::operator=(const Polygon& rhs) {
    Corpse::operator=(rhs);
    this->points_number = rhs.get_points_number();
    this->points = rhs.get_points();
    this->relative_points = rhs.get_relative_points();
    // TODO COPY Triangulation!!!
    // this->triangulation =
    // std::vector<std::vector<std::shared_ptr<sf::Vector2f>>> triangulation;
    return *this;
}
Polygon::~Polygon() {}

const int Polygon::get_class() { return ID_POLYGON; }

void Polygon::Move(float x, float y, bool relative) {
    if (relative) {
        this->current_pos = this->current_pos + sf::Vector2f(x, y);
    } else {
        this->current_pos = sf::Vector2f(x, y);
    }
    this->update_points();
}
void Polygon::Move(sf::Vector2f move, bool relative) {
    if (relative) {
        this->current_pos = this->current_pos + move;
    } else {
        this->current_pos = move;
    }
    this->update_points();
}

bool Polygon::inBounds(float x1, float x2, float y1, float y2) { return true; }
bool Polygon::Pointed(float x, float y) {
    // Ray Casting Algorithm
    std::vector<float> sizes = this->get_sides_size();
    float ray_lenght = ftn::Length(this->get_pos() - sf::Vector2f(x, y)) + *std::max_element(std::begin(sizes), std::end(sizes));

    sf::Vector2f rayA = {x, y};
    sf::Vector2f rayB = {x + ray_lenght, y};  // ftn::Normalize(this->get_pos()-sf::Vector2f(x, y))*ray_lenght;
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = this->get_sides();
    int intersections = 0;
    for (int i = 0; i < sides.size(); i++) {
        if (ftn::Segments_Intersect(rayA, rayB, sides.at(i).first, sides.at(i).second)) { intersections++; }
    }
    if ((intersections & 1) == 1) { return true; }
    return false;
}

void Polygon::Collision(std::shared_ptr<Corpse> a) {
    if (Circle* circle = dynamic_cast<Circle*>(a.get())) {
        // Polygon / Circle collision
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = this->get_sides();

        // Collide if the center of the circle is in the polygon
        if (this->Pointed(circle->get_pos().x, circle->get_pos().y)) {
            // Find the closest point on edges
            std::pair<sf::Vector2f, sf::Vector2f> closest_side = ftn::Closest_Edge(sides, circle->get_pos());

            sf::Vector2f closest_projection = ftn::Segment_Projection(closest_side.first, closest_side.second, circle->get_pos());
            sf::Vector2f vector_response = ftn::Normalize(ftn::Norme(closest_side.second, closest_side.first)) * (ftn::Length(circle->get_pos(), closest_projection) + circle->get_size());
            Corpse::CollisionResponse(this, circle, vector_response);
            return;
        }

        for (int i = 0; i < sides.size(); i++) {
            auto test_intersect = ftn::Line_Circle_Intersect(sides.at(i).first, sides.at(i).second, circle->get_pos(), circle->get_size());

            // Don't collide with any edge
            if (test_intersect.first == 0) { continue; }
            if (test_intersect.first == 1) {
                // Collide at the middle of an edge
                sf::Vector2f vector_response = ftn::Normalize(circle->get_pos() - test_intersect.second) * (ftn::Length(circle->get_pos(), test_intersect.second) - circle->get_size());
                Corpse::CollisionResponse(this, circle, vector_response);
                return;

            } else if (test_intersect.first == 2) {
                // Collide with the first point of the edge (current edge + last edge)
                int last_edge = (i - 1) % sides.size();
                sf::Vector2f normals_average = circle->get_pos() - sides.at(last_edge).second;
                sf::Vector2f vector_response = ftn::Normalize(normals_average) * (ftn::Length(circle->get_pos(), sides.at(i).first) - circle->get_size());
                Corpse::CollisionResponse(this, circle, vector_response);
                return;

            } else if (test_intersect.first == 3) {
                // Collide with the second point of the edge (current edge + next edge)
                int next_edge = (i + 1) % sides.size();
                sf::Vector2f normals_average = circle->get_pos() - sides.at(i).second;
                sf::Vector2f vector_response = ftn::Normalize(normals_average) * (ftn::Length(circle->get_pos(), sides.at(i).second) - circle->get_size());
                Corpse::CollisionResponse(this, circle, vector_response);
                return;
            }
        }

    } else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {
        // Polygon / Polygon collision (Separating axis theorem)

        // Make separating axis (perpendicular to the line that pass by the two objects center)
        sf::Vector2f axis = ftn::Norme(this->get_pos(), polygon->get_pos());  // sf::Vector2f axis = ftn::Norme(this->get_diff_pos(), polygon->get_diff_pos());

        // Find the two farthest points of the projection points on the separator axis
        std::vector<float> self_projections = std::vector<float>();
        std::vector<float> other_projections = std::vector<float>();

        for (int i = 0; i < this->get_points_number(); i++) { self_projections.push_back(ftn::Dot(this->get_points().at(i), axis)); }
        for (int i = 0; i < polygon->get_points_number(); i++) { other_projections.push_back(ftn::Dot(polygon->get_points().at(i), axis)); }

        const auto self_minmax = std::minmax_element(self_projections.begin(), self_projections.end());
        const auto other_minmax = std::minmax_element(other_projections.begin(), other_projections.end());

        float self_min = *self_minmax.first;
        float self_max = *self_minmax.second;
        float other_min = *other_minmax.first;
        float other_max = *other_minmax.second;
    }
}

void Polygon::update_points() {
    for (int i = 0; i < this->points_number; i++) { this->points.at(i) = this->get_pos() + this->relative_points.at(i); }
}

void Polygon::triangulate() {
    /* Reset the triangulation shape */
    this->triangulation = std::vector<std::vector<std::shared_ptr<sf::Vector2f>>>();

    if (this->is_convex()) {
        /* If the polygon is convex, no need to decompose it for the collision to work properly */
        std::vector<std::shared_ptr<sf::Vector2f>> triangle = std::vector<std::shared_ptr<sf::Vector2f>>();
        for (int i = 0; i < this->points_number; i++) {
            /* /!\ IT COPY THE POINTS INSTEAD OF LINKING THEM... */
            triangle.push_back(std::make_shared<sf::Vector2f>(std::forward<sf::Vector2f>(this->points.at(i))));
        }
        this->triangulation.push_back(triangle);
    } else {
        std::vector<std::shared_ptr<sf::Vector2f>> triangle = std::vector<std::shared_ptr<sf::Vector2f>>();

        /* Ear clipping / Triangulation */
        // TODO

        this->triangulation.push_back(triangle);
    }
}

std::vector<sf::Vector2f> Polygon::init_relative_points(std::vector<sf::Vector2f> points) {
    sf::Vector2f pos = phy::Polygon::compute_center(points);
    this->set_pos(pos);
    std::vector<sf::Vector2f> relative_points = std::vector<sf::Vector2f>();
    for (int i = 0; i < points.size(); i++) { relative_points.push_back(points.at(i) - pos); }
    return relative_points;
}

void Polygon::set_points(std::vector<sf::Vector2f> points) { this->points = points; }

void Polygon::add_point(sf::Vector2f point) {
    this->points.push_back(point);
    this->points_number++;

    sf::Vector2f diff_pos = this->current_pos - this->last_pos;
    sf::Vector2f computed_center = phy::Polygon::compute_center(this->points);

    this->set_pos(computed_center);
    this->last_pos = computed_center - diff_pos;
    this->relative_points = init_relative_points(this->points);
    this->update_points();
    this->triangulate();
}

void Polygon::remove_point(int i) {}

sf::Vector2f Polygon::compute_center(std::vector<sf::Vector2f> points) {
    // return ftn::Points_Average(this->points);
    return ftn::Centroid(this->get_sides());
}

void Polygon::Step() {
    if (this->fixed) {
        this->last_pos = this->current_pos;
    } else {
        sf::Vector2f diff_pos = this->current_pos - this->last_pos;
        this->last_pos = this->current_pos;
        this->current_pos = this->current_pos + diff_pos;
    }

    if (this->tied) {
        this->last_rotation = std::fmod(this->current_rotation, 360);
    } else {
        float diff_rotation = std::fmod(this->current_rotation - this->last_rotation, 360);
        this->last_rotation = this->current_rotation;
        this->current_rotation = std::fmod(this->current_rotation + diff_rotation, 360);
        for (int i = 0; i < this->relative_points.size(); i++) { ftn::Rotate(this->relative_points.at(i), current_rotation); }
    }

    if (!ftn::decimal_equals(motor_rotation, 0.0f, 0.0001f)) {
        // Add the motor rotation even if the object is tied
        this->current_rotation = this->current_rotation + motor_rotation;
        for (int i = 0; i < this->relative_points.size(); i++) { ftn::Rotate(this->relative_points.at(i), motor_rotation); }
    }

    this->update_points();
}
void Polygon::Stop() {
    this->last_pos = this->current_pos;
    this->last_rotation = this->current_rotation;
}

ftn::Rectangle Polygon::get_corpse_bounds() const {
    const std::vector<sf::Vector2f> points = this->points;
    const auto min_max_x = std::minmax_element(points.begin(), points.end(), [](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.x < rhs.x; });
    const auto min_max_y = std::minmax_element(points.begin(), points.end(), [](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.y < rhs.y; });

    float min_x = (*min_max_x.first).x;
    float max_x = (*min_max_x.second).x;
    float min_y = (*min_max_y.first).y;
    float max_y = (*min_max_y.second).y;

    return {sf::Vector2f(min_x, min_y), sf::Vector2f(max_x - min_x, max_y - min_y)};
}

int Polygon::get_points_number() const { return this->points_number; }
std::vector<sf::Vector2f> Polygon::get_points() const { return this->points; }

std::vector<sf::Vector2f> Polygon::get_relative_points() const { return this->relative_points; }
void Polygon::set_relative_points(std::vector<sf::Vector2f> relative_points) { this->relative_points = relative_points; }

std::vector<float> Polygon::get_sides_size() const {
    std::vector<sf::Vector2f> sides = this->get_sides_val();
    std::vector<float> sizes = std::vector<float>();
    for (int i = 0; i < sides.size(); i++) { sizes.push_back(ftn::Length(sides.at(i))); }
    return sizes;
}
std::vector<sf::Vector2f> Polygon::get_sides_val() const {
    std::vector<sf::Vector2f> sides = std::vector<sf::Vector2f>();
    if (this->points_number > 1) {
        for (int i = 0; i < this->points_number - 1; i++) { sides.push_back(this->points.at(i + 1) - this->points.at(i)); }
        sides.push_back(this->points.at(0) - this->points.at(this->points_number - 1));
    }
    return sides;
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Polygon::get_sides() const {
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> pairs = std::vector<std::pair<sf::Vector2f, sf::Vector2f>>();
    if (this->points.size() > 1) {
        for (int i = 0; i < this->points.size() - 1; i++) { pairs.push_back({this->points.at(i), this->points.at(i + 1)}); }
        pairs.push_back({this->points.at(this->points.size() - 1), this->points.at(0)});
    }
    return pairs;
}

std::vector<std::vector<std::shared_ptr<sf::Vector2f>>> Polygon::get_triangulation() const { return this->triangulation; }

bool Polygon::is_convex() const {
    /*
        Check if the polygon is not convex
        - Check if 2 edges intersect ?
        - Check for all angles if they are > 180°/2PI
    */

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = this->get_sides();

    /* We check for every edges that don't have an edge in common (opposites) if they intersect */
    if (sides.size() <= 3) { return true; } /* triangles are always convex */

    for (int i = 0; i < sides.size() - 2; i++) {
        for (int j = i + 2; j < sides.size() - (i == 0); j++) {
            std::pair<sf::Vector2f, sf::Vector2f> sideA = sides.at(i);
            std::pair<sf::Vector2f, sf::Vector2f> sideB = sides.at(j);
            if (ftn::Segments_Intersect(sideA.first, sideA.second, sideB.first, sideB.second)) { return false; }
        }
    }

    std::vector<sf::Vector2f> points = this->get_points();

    // tests angles <= 2PI...
    for (int i = 0; i < points.size() - 2; i++) {
        if (ftn::angle(points.at(i), points.at(i + 1), points.at(i + 2)) > 180.0f) { return false; }
    }

    if (ftn::angle(points.at(points.size() - 1), points.at(0), points.at(1)) > 180.0f) { return false; }

    return true;
}

}  // namespace phy
