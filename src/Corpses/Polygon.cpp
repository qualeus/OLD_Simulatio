#include "../../include/Corpses/Polygon.hpp"

namespace phy {

Polygon::Polygon(std::initializer_list<gmt::VectorI> points, gmt::UnitI mass, gmt::UnitI damping, gmt::UnitI speed_x, gmt::UnitI speed_y, gmt::UnitI rotation, gmt::UnitI motor, bool fixed, bool tied, bool etherial, sf::Color color) : Corpse(mass, damping, fixed, tied, etherial, color) {
    std::vector<gmt::VectorI> vect_points(std::begin(points), std::end(points));
    std::vector<std::shared_ptr<gmt::VectorI>> shared_points = {};
    for (int i = 0; i < vect_points.size(); i++) { shared_points.push_back(std::make_shared<gmt::VectorI>(vect_points.at(i))); }

    this->points = gmt::VerticesI(shared_points);
    this->points.Reorder();
    this->polygons = this->points.Triangulate();

    gmt::VectorI centroid = this->points.Centroid();

    this->current_pos = centroid;
    this->last_pos = gmt::VectorI(centroid.x - speed_x, centroid.y - speed_y);

    this->current_rotation = gmt::UnitI(0);
    this->last_rotation = rotation;

    this->motor = motor;
    this->propulsor = propulsor;
}
Polygon& Polygon::operator=(const Polygon& rhs) {
    Corpse::operator=(rhs);
    this->points = rhs.get_points();
    this->polygons = rhs.get_polygons();
    return *this;
}
Polygon::~Polygon() {}

const int Polygon::get_class() { return ID_POLYGON; }
int Polygon::id_class() { return ID_POLYGON; }

void Polygon::Move(const gmt::VectorI& move) {
    this->points.Translate(move - this->current_pos);
    this->current_pos = move;
}
void Polygon::Drag(const gmt::VectorI& drag) {
    this->points.Translate(drag);
    this->current_pos = this->current_pos + drag;
}

void Polygon::Turn(const gmt::UnitI& turn) {
    this->points.Rotate(this->current_rotation - turn, this->current_pos);
    this->current_rotation = std::fmod(turn, gmt::UnitI(RO));
}
void Polygon::Rotate(const gmt::UnitI& rotate) {
    this->points.Rotate(rotate, this->current_pos);
    this->current_rotation = std::fmod(this->current_rotation + rotate, gmt::UnitI(RO));
}

bool Polygon::inBounds(const gmt::BoundsI& bounds) const { return gmt::BoundsI::BoundsInBounds(this->get_corpse_bounds(), bounds); }
bool Polygon::Pointed(const gmt::VectorI& point) const { return gmt::VerticesI::PointInShape(this->get_points(), point); }

void Polygon::add_point(gmt::VectorI point) {
    this->points.vertices.push_back(std::make_shared<gmt::VectorI>(point));
    // this->points.Reorder();
    this->polygons = this->points.Triangulate();

    gmt::VectorI computed_center = this->points.Centroid();
    gmt::VectorI diff_pos = computed_center - this->current_pos;

    this->current_pos = computed_center;
    this->last_pos = last_pos + diff_pos;
}

void Polygon::remove_point(int i) {
    // this->points.vertices.push_back(std::make_shared<gmt::VectorI>(point));
    // this->points.Reorder();
    this->polygons = this->points.Triangulate();

    gmt::VectorI computed_center = this->points.Centroid();
    gmt::VectorI diff_pos = computed_center - this->current_pos;

    this->current_pos = computed_center;
    this->last_pos = last_pos + diff_pos;
}

void Polygon::Step() {
    if (this->fixed) {
        this->last_pos = this->current_pos;
    } else {
        gmt::VectorI diff_pos = this->current_pos - this->last_pos;
        this->last_pos = this->current_pos;
        this->Drag(diff_pos);
    }

    if (this->tied) {
        this->last_rotation = std::fmod(this->current_rotation, gmt::UnitI(RO));
    } else {
        gmt::UnitI diff_rotation = this->current_rotation - this->last_rotation;
        this->last_rotation = this->current_rotation;
        this->Rotate(diff_rotation);
    }

    // Add the motor rotation even if the object is tied
    // if (!gmt::decimal_equals(motor, 0.0f, 0.0001f)) { this->current_rotation = this->current_rotation + motor; }
}
void Polygon::Stop() { this->last_pos = this->current_pos; }
void Polygon::Bloc() { this->last_rotation = this->current_rotation; }

gmt::BoundsI Polygon::get_corpse_bounds() const { return this->points.Bounds(); }

gmt::VerticesI Polygon::get_points() const { return this->points; }
void Polygon::set_points(gmt::VerticesI points) { this->points = points; }

std::vector<gmt::VerticesI> Polygon::get_polygons() const { return this->polygons; }
void Polygon::set_polygons(std::vector<gmt::VerticesI> polygons) { this->polygons = polygons; }

std::vector<std::pair<std::shared_ptr<gmt::VectorI>, std::shared_ptr<gmt::VectorI>>> Polygon::get_sides() const { return this->points.Pairs(); }

}  // namespace phy
