#include "../include/System.hpp"

namespace phy {

System::System(bool gravity, float force_x, float force_y, float limit_x, float limit_y) {
    this->gravity = gravity;
    this->force_x = force_x;
    this->force_y = force_y;

    this->corpses = std::vector<std::shared_ptr<Corpse>>();
    this->pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
    this->quad_pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();

    this->quadtree = gmt::Quadtree({sf::Vector2f(-limit_x / 2.0f, -limit_y / 2.0f), sf::Vector2f(limit_x, limit_y)}, 1);

    this->limits = {sf::Vector2f(-(AROUND_QUADTREE + limit_x) / 2.0f, -(AROUND_QUADTREE + limit_y) / 2.0f), sf::Vector2f(limit_x + AROUND_QUADTREE, limit_y + AROUND_QUADTREE)};
    Prepare();
}

System& System::operator=(const System& rhs) {
    this->corpses_size = rhs.get_corpses_size();
    this->pairs_size = rhs.get_pairs_size();

    this->force_x = rhs.get_force_x();
    this->force_y = rhs.get_force_y();
    this->gravity = rhs.get_gravity();
    this->LS = rhs.get_LS();
    this->G = rhs.get_G();

    this->dt = rhs.get_dt();
    this->collision_accuracy = rhs.get_collision_accuracy();
    this->constraint_accuracy = rhs.get_constraint_accuracy();

    gmt::Rectangle limits = rhs.get_limits();

    this->corpses = std::vector<std::shared_ptr<Corpse>>();
    this->pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
    this->quad_pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
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

void System::Prepare() { InitQuadtree(); }

void System::Step() {
    // Update Positions
    CorpsesStep();
    // Update Forces
    // Update Velocities
    // Apply Boundaries conditions
    for (int i = 0; i < collision_accuracy; i++) {
        StepQuadtree();
        QuadPairsStep();
    }
    PairsStep();
    CheckLimits();
    // Move Global Time
    UpdateTime();
    // Calculate the Output
}

void System::UpdateTime() { this->t += this->dt; }

void System::CheckLimits() {
    for (int i = 0; i < corpses_size; i++) {
        if (phy::Circle* circle = dynamic_cast<phy::Circle*>(get_corpse(i).get())) {
            if (gmt::rect_out_bounds(circle->get_corpse_bounds(), get_limits())) { get_corpse(i)->Remove(); }
        } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(get_corpse(i).get())) {
            if (gmt::rect_out_bounds(polygon->get_corpse_bounds(), get_limits())) { get_corpse(i)->Remove(); }
        }
    }
}

void System::CorpsesStep() {
    for (int i = 0; i < corpses_size; i++) {
        if (get_corpse(i)->get_removed()) { continue; }  // Removed

        get_corpse(i)->Step();
        if (!get_corpse(i)->get_fixed()) { get_corpse(i)->Move(sf::Vector2f(this->force_x, this->force_y) * this->dt * this->dt); }
    }
}

void System::CorpseStop(int i) {
    std::shared_ptr<Corpse> corpse = get_corpse(i);

    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(corpse.get())) {
        circle->Stop();
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(corpse.get())) {
        polygon->Stop();
    }
}

void System::PairsStep() {
    for (int i = 0; i < pairs_size; i++) {
        if (this->gravity) { Gravity(get_pair_A(i), get_pair_B(i)); }
        // Collision(get_pair_A(i), get_pair_B(i)); old collision system
    }
}

void System::QuadPairsStep() {
    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> quadpairs = this->quadtree.make_pairs();
    this->quad_pairs = quadpairs;
    for (int i = 0; i < quadpairs.size(); i++) { Collision(quadpairs.at(i).first, quadpairs.at(i).second); }
}

void System::Collision(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {
    if (a->get_removed() || b->get_removed()) { return; }  // Removed

    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(a.get())) {
        circle->Collision(b);
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(a.get())) {
        polygon->Collision(b);
    }
}

void System::Gravity(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {
    if (a->get_removed() || b->get_removed()) { return; }  // One Removed
    if (a->get_fixed() && b->get_fixed()) { return; }      // Both Fixed

    float dist = gmt::Length(a->get_pos_x(), a->get_pos_y(), b->get_pos_x(), b->get_pos_y());

    // Possible optimisation: Remove the multiplication and divisio, by the mass and
    // just multiply the force by the other body mass when applying it

    float force = G * a->get_mass() * b->get_mass() / (dist * dist);  // G * (ma * mb)/(r^2)
    if (force > LS) { force = LS; }                                   // Limit with the Light Speed

    sf::Vector2f diff = sf::Vector2f(b->get_pos_x() - a->get_pos_x(), b->get_pos_y() - a->get_pos_y()) / dist;
    sf::Vector2f acceleration_a = diff * force / a->get_mass();
    sf::Vector2f acceleration_b = -diff * force / b->get_mass();

    if (!a->get_fixed()) { a->Move(acceleration_a * this->dt * this->dt); }
    if (!b->get_fixed()) { b->Move(acceleration_b * this->dt * this->dt); }
}

void System::InitQuadtree() { StepQuadtree(); }

void System::StepQuadtree() {
    this->quadtree.clear();
    for (int i = 0; i < corpses_size; i++) {
        if (get_corpse(i)->get_removed()) { continue; }  // Removed
        this->quadtree.insert(get_corpse(i));
    }
}

std::shared_ptr<gmt::Quadtree> System::get_quadtree() { return std::make_shared<gmt::Quadtree>(this->quadtree); }

float System::get_dt() const { return this->dt; }
void System::set_dt(float dt) {
    // We compute the change in time
    float dt_diff = dt - this->dt;

    // We need to avoid a dt to close to 0 because
    // it mess up with the corpses velocities.
    // It's better to just pass the area around 0.
    if (gmt::float_equals(dt, 0.0f, dt_diff * 0.1f)) {
        dt = dt + dt_diff;
        dt_diff = dt_diff * 2.0f;
    }

    float dt_frac = dt / this->dt;
    // Update corpses velocities an rotations
    for (int i = 0; i < corpses_size; i++) {
        if (get_corpse(i)->get_removed()) { continue; }  // Removed
        get_corpse(i)->set_last_pos(get_corpse(i)->get_pos() - get_corpse(i)->get_diff_pos() * dt_frac);
        get_corpse(i)->set_last_rotation(get_corpse(i)->get_rotation() - get_corpse(i)->get_diff_rotation() * dt_frac);
    }

    this->dt = dt;
}

double System::get_t() const { return this->t; }
void System::set_t(double t) { this->t = t; }

float System::get_force_x() const { return this->force_x; }
void System::set_force_x(float force_x) { this->force_x = force_x; }

float System::get_force_y() const { return this->force_y; }
void System::set_force_y(float force_y) { this->force_y = force_y; }

bool System::get_gravity() const { return this->gravity; }
void System::set_gravity(bool gravity) { this->gravity = gravity; }

float System::get_LS() const { return this->LS; }
void System::set_LS(float LS) { this->LS = LS; }

float System::get_G() const { return this->G; }
void System::set_G(float G) { this->G = G; }

int System::get_collision_accuracy() const { return this->collision_accuracy; }
void System::set_collision_accuracy(int collision_accuracy) { this->collision_accuracy = collision_accuracy; }

int System::get_constraint_accuracy() const { return this->constraint_accuracy; }
void System::set_constraint_accuracy(int constraint_accuracy) { this->constraint_accuracy = constraint_accuracy; }

int System::get_corpses_size() const { return this->corpses_size; }
int System::get_pairs_size() const { return this->pairs_size; }
int System::get_quad_pairs_size() const { return this->quad_pairs.size(); }

void System::addCorpse(Polygon poly) {
    this->polygons.push_back(poly);
    add_corpse(std::make_shared<Polygon>(this->polygons.at(this->polygons.size() - 1)));
}
void System::addCorpse(Circle circ) {
    this->circles.push_back(circ);
    add_corpse(std::make_shared<Circle>(this->circles.at(this->circles.size() - 1)));
}

void System::add_corpse(std::shared_ptr<Corpse> a) {
    this->corpses.emplace_back(std::move(a));
    this->corpses_size++;  // Update the size of the array = n
    if (corpses_size > 1) {
        for (int b_index = 0; b_index < corpses_size - 1; b_index++) { System::add_pair(get_corpse(this->corpses_size - 1), get_corpse(b_index)); }
    }
}

void System::add_pair(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {
    this->pairs.push_back({a, b});
    this->pairs_size++;  // Update the size of the array = [n(n-1)]/2
}

gmt::Rectangle System::get_limits() const { return this->limits; }
void System::set_limits(gmt::Rectangle limits) { this->limits = limits; }

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