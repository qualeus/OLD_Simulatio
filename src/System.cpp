#include "../include/System.hpp"

namespace phy {

System::System(bool gravity, gmt::UnitI force_x, gmt::UnitI force_y, gmt::UnitI limit_x, gmt::UnitI limit_y) {
    this->gravity = gravity;
    this->force_x = force_x;
    this->force_y = force_y;

    this->corpses = std::vector<std::shared_ptr<Corpse>>();
    this->pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
    this->quad_pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
    this->collisions = std::vector<gmt::CollisionI>();

    this->quadtree = gmt::QuadtreeI(gmt::BoundsI(-limit_x / gmt::UnitI(2), -limit_y / gmt::UnitI(2), limit_x / gmt::UnitI(2), limit_y / gmt::UnitI(2)), gmt::UnitI(1));

    gmt::UnitI mid_limx = limit_x / gmt::UnitI(2);
    gmt::UnitI mid_limy = limit_y / gmt::UnitI(2);

    this->limits = gmt::BoundsI(-mid_limx, -mid_limy, mid_limx, mid_limy);
    StepQuadtree();
}

System& System::operator=(const System& rhs) {
    this->force_x = rhs.get_force_x();
    this->force_y = rhs.get_force_y();
    this->gravity = rhs.get_gravity();
    this->LS = rhs.get_LS();
    this->G = rhs.get_G();

    this->dt = rhs.get_dt();
    this->collision_accuracy = rhs.get_collision_accuracy();
    this->constraint_accuracy = rhs.get_constraint_accuracy();

    gmt::BoundsI limits = rhs.get_limits();

    this->corpses = std::vector<std::shared_ptr<Corpse>>();
    this->pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
    this->quad_pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
    this->collisions = std::vector<gmt::CollisionI>();
    this->quadtree = rhs.quadtree;

    for (int i = 0; i < rhs.get_corpses_size(); i++) {
        // Populate the corpses array
        std::shared_ptr<phy::Corpse> temp_corpse = rhs.get_corpse(i);
        if (phy::Circle* circle = dynamic_cast<phy::Circle*>(temp_corpse.get())) {
            Circle circle_copy;
            circle_copy = *circle;
            this->corpses.push_back(std::make_shared<phy::Circle>(circle_copy));
        } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(temp_corpse.get())) {
            Polygon polygon_copy;
            polygon_copy = *polygon;
            this->corpses.push_back(std::make_shared<phy::Polygon>(polygon_copy));
        }

        // Populate the pairs array
        if (rhs.get_corpses_size() > 1) {
            for (int j = 0; j < i; j++) { this->pairs.push_back({this->corpses.at(i), this->corpses.at(j)}); }
        }
    }

    return *this;
}

System::~System() {}

void System::Step() {
    // Update Positions
    CorpsesStep();
    // Update Forces
    // Update Velocities
    // Apply Boundaries conditions
    for (int i = 0; i < collision_accuracy; i++) {
        // StepQuadtree();
        // QuadPairsStep();
    }
    PairsStep();
    CheckLimits();
    // Move Global Time
    UpdateTime();
    // Calculate the Output
}

void System::UpdateTime() { this->t += this->dt; }

void System::CheckLimits() {
    for (int i = 0; i < corpses.size(); i++) {
        if (phy::Circle* circle = dynamic_cast<phy::Circle*>(get_corpse(i).get())) {
            if (gmt::BoundsI::BoundsOutBounds(circle->get_corpse_bounds(), get_limits())) { get_corpse(i)->Remove(); }
        } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(get_corpse(i).get())) {
            if (gmt::BoundsI::BoundsOutBounds(polygon->get_corpse_bounds(), get_limits())) { get_corpse(i)->Remove(); }
        }
    }
}

void System::CorpsesStep() {
    for (int i = 0; i < corpses.size(); i++) {
        if (get_corpse(i)->get_removed()) { continue; }  // Removed

        get_corpse(i)->Step();
        if (!get_corpse(i)->get_fixed()) { get_corpse(i)->Drag(gmt::VectorI(this->force_x, this->force_y) * this->dt * this->dt); }
    }
}

void System::CorpseStop(int i) {
    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(get_corpse(i).get())) {
        circle->Stop();
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(get_corpse(i).get())) {
        polygon->Stop();
    }
}

void System::PairsStep() {
    this->collisions = {};  // MOVE IN QUAD COLLISION

    for (int i = 0; i < pairs.size(); i++) {
        gmt::CollisionI::Resolve(get_pair_A(i), get_pair_B(i));  // OLD COLLISION
        if (this->gravity) { Gravity(get_pair_A(i), get_pair_B(i)); }
    }
}

void System::QuadPairsStep() {
    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> quadpairs = this->quadtree.make_pairs();

    // We Store the Quad Pairs before resolving the collisions
    this->quad_pairs = quadpairs;

    for (int i = 0; i < quadpairs.size(); i++) { this->collisions.push_back(gmt::CollisionI::Resolve(quadpairs.at(i).first, quadpairs.at(i).second)); }
}

void System::Gravity(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {
    if (a->get_removed() || b->get_removed()) { return; }  // One Removed
    if (a->get_fixed() && b->get_fixed()) { return; }      // Both Fixed

    gmt::UnitI dist = gmt::VectorI::Distance(a->get_pos(), b->get_pos());

    // Possible optimisation: Remove the multiplication and division by the mass and
    // just multiply the force by the other body mass when applying it

    gmt::UnitI force = G * a->get_mass() * b->get_mass() / (dist * dist);  // G * (ma * mb)/(r^2)
    if (force > LS) { force = LS; }                                        // Limit with the Light Speed

    gmt::VectorI diff = gmt::VectorI(b->get_pos_x() - a->get_pos_x(), b->get_pos_y() - a->get_pos_y()) / dist;
    gmt::VectorI acceleration_a = diff * force / a->get_mass();
    gmt::VectorI acceleration_b = -diff * force / b->get_mass();

    if (!a->get_fixed()) { a->Drag(acceleration_a * this->dt * this->dt); }
    if (!b->get_fixed()) { b->Drag(acceleration_b * this->dt * this->dt); }
}

void System::StepQuadtree() {
    this->quadtree.Clear();
    for (int i = 0; i < corpses.size(); i++) {
        if (get_corpse(i)->get_removed()) { continue; }  // Removed
        this->quadtree.Insert(get_corpse(i));
    }
}

std::shared_ptr<gmt::QuadtreeI> System::get_quadtree() { return std::make_shared<gmt::QuadtreeI>(this->quadtree); }

gmt::UnitI System::get_dt() const { return this->dt; }
void System::set_dt(gmt::UnitI dt) {
    // We compute the change in time
    gmt::UnitI dt_diff = dt - this->dt;

    // We need to avoid a dt to close to 0 because
    // it mess up with the corpses velocities.
    // It's better to just pass the area around 0.
    if (!gmt::float_equals(dt, gmt::UnitI(0), dt_diff / gmt::UnitI(10))) {
        dt = dt + dt_diff;
        dt_diff = dt_diff * gmt::UnitI(2);
    }

    gmt::UnitI dt_frac = dt / this->dt;

    for (int i = 0; i < corpses.size(); i++) {
        if (get_corpse(i)->get_removed()) { continue; }  // Removed

        // Update corpses velocities an rotations
        get_corpse(i)->set_last_pos(get_corpse(i)->get_pos() - get_corpse(i)->get_diff_pos() * dt_frac);
        get_corpse(i)->set_last_rotation(get_corpse(i)->get_rotation() - get_corpse(i)->get_diff_rotation() * dt_frac);
    }

    this->dt = dt;
}

double System::get_t() const { return this->t; }
void System::set_t(double t) { this->t = t; }

gmt::UnitI System::get_force_x() const { return this->force_x; }
void System::set_force_x(gmt::UnitI force_x) { this->force_x = force_x; }

gmt::UnitI System::get_force_y() const { return this->force_y; }
void System::set_force_y(gmt::UnitI force_y) { this->force_y = force_y; }

bool System::get_gravity() const { return this->gravity; }
void System::set_gravity(bool gravity) { this->gravity = gravity; }

gmt::UnitI System::get_LS() const { return this->LS; }
void System::set_LS(gmt::UnitI LS) { this->LS = LS; }

gmt::UnitI System::get_G() const { return this->G; }
void System::set_G(gmt::UnitI G) { this->G = G; }

int System::get_collision_accuracy() const { return this->collision_accuracy; }
void System::set_collision_accuracy(int collision_accuracy) { this->collision_accuracy = collision_accuracy; }

int System::get_constraint_accuracy() const { return this->constraint_accuracy; }
void System::set_constraint_accuracy(int constraint_accuracy) { this->constraint_accuracy = constraint_accuracy; }

int System::get_corpses_size() const { return this->corpses.size(); }
int System::get_pairs_size() const { return this->pairs.size(); }
int System::get_quad_pairs_size() const { return this->quad_pairs.size(); }

void System::addCorpse(Polygon polygon) { add_corpse(std::make_shared<Polygon>(polygon)); }
void System::addCorpse(Circle circle) { add_corpse(std::make_shared<Circle>(circle)); }

void System::add_corpse(std::shared_ptr<Corpse> corpse) {
    this->corpses.emplace_back(std::move(corpse));  // size of the array = [n]
    if (corpses.size() > 1) {
        for (int b_index = 0; b_index < corpses.size() - 1; b_index++) { System::add_pair(get_corpse(this->corpses.size() - 1), get_corpse(b_index)); }
    }
}

void System::add_pair(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {
    this->pairs.push_back({a, b});  // size of the array = [n(n-1)]/2
}

gmt::BoundsI System::get_limits() const { return this->limits; }
void System::set_limits(gmt::BoundsI limits) { this->limits = limits; }

std::vector<std::shared_ptr<Corpse>> System::get_corpses() const { return this->corpses; }
std::shared_ptr<Corpse> System::get_corpse(int index) const {
    if (index >= 0 && index < get_corpses_size()) {
        return this->corpses.at(index);
    } else {
        return nullptr;
    }
}

std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> System::get_pairs() const { return this->pairs; }
std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> System::get_pair(int index) const {
    if (index >= 0 && index < get_pairs_size()) {
        return this->pairs.at(index);
    } else {
        return {nullptr, nullptr};
    }
}
std::shared_ptr<Corpse> System::get_pair_A(int index) const {
    if (index >= 0 && index < get_pairs_size()) {
        return this->pairs.at(index).first;
    } else {
        return nullptr;
    }
}
std::shared_ptr<Corpse> System::get_pair_B(int index) const {
    if (index >= 0 && index < get_pairs_size()) {
        return this->pairs.at(index).second;
    } else {
        return nullptr;
    }
}

std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> System::get_quad_pair(int index) const {
    if (index >= 0 && index < this->quad_pairs.size()) {
        return this->quad_pairs.at(index);
    } else {
        return {nullptr, nullptr};
    }
}

}  // namespace phy