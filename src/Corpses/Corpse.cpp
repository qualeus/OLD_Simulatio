#include "../../include/Corpses/Corpse.hpp"
#define MIN_DAMPING 0.1
namespace phy {

Corpse::Corpse(gmt::UnitI mass, gmt::UnitI damping, bool fixed, bool tied, bool etherial, sf::Color color) {
    static int global_corpse_id = 0;
    this->id = global_corpse_id++;

    this->fixed = fixed;
    this->tied = tied;
    this->etherial = etherial;
    this->removed = false;

    this->mass = mass;
    this->friction = gmt::UnitI(1);
    this->damping = std::min(damping, gmt::UnitI(MIN_DAMPING));
    this->color = color;
}
Corpse& Corpse::operator=(const Corpse& rhs) {
    this->id = rhs.get_id();

    this->fixed = rhs.get_fixed();
    this->tied = rhs.get_tied();
    this->etherial = rhs.get_etherial();
    this->removed = rhs.get_removed();

    this->mass = rhs.get_mass();
    this->friction = rhs.get_friction();
    this->damping = rhs.get_damping();
    this->color = rhs.get_color();

    this->current_pos = rhs.get_pos();
    this->last_pos = rhs.get_last_pos();
    this->propulsor = rhs.get_propulsor();

    this->current_rotation = rhs.get_rotation();
    this->last_rotation = rhs.get_last_rotation();
    this->motor = rhs.get_motor();

    return *this;
}
Corpse::~Corpse() = default;

int Corpse::get_id() const { return this->id; }
int Corpse::get_class() const { return ID_CORPSE; }

bool Corpse::get_fixed() const { return this->fixed; }
void Corpse::set_fixed(bool fixed) { this->fixed = fixed; }

bool Corpse::get_etherial() const { return this->etherial; }
void Corpse::set_etherial(bool etherial) { this->etherial = etherial; }

bool Corpse::get_tied() const { return this->tied; }
void Corpse::set_tied(bool tied) { this->tied = tied; }

bool Corpse::get_removed() const { return this->removed; }
void Corpse::Remove() { this->removed = true; }

void Corpse::Step() {}
void Corpse::Stop() {}
void Corpse::Move(gmt::UnitI x, gmt::UnitI y, bool relative) {}
void Corpse::Move(gmt::VectorI move, bool relative) {}
void Corpse::Collision(std::shared_ptr<Corpse> a) {}

void Corpse::CollisionResponse(phy::Corpse* corpse_a, phy::Corpse* corpse_b, const gmt::VectorI& vect_force) {
    if (corpse_a->get_etherial()) { return; }
    if (corpse_b->get_etherial()) { return; }
    gmt::UnitI damping = (corpse_a->get_bounce() + corpse_b->get_bounce()) / gmt::UnitI(2);  // Damping is evenly distributed among the corpses

    /*
        TEMP: TODO
        - recompute the damping with the proper forces applied
        - push the corpse out of the other shape with the normal vector and the force passed by parameter (vect_force)
            => BUT DON'T CHANCE THE VELOCITY (how to do? last pos?)
        - Then apply the vector (2m2v¯2 + (m1 −m2)v¯1)/(2m2v¯2 + (m1 −m2)v¯1)

    */
    // Test if the collision is asymetric (Fixed/Not Fixed) / or if the two corpses are Fixed
    if (corpse_a->get_fixed() || corpse_b->get_fixed()) {
        if (!corpse_a->get_fixed()) {
            // corpse_a is Not Fixed and corpse_b is Fixed:
            // gmt::VectorI temp_pos = corpse_a->get_last_pos();

            // corpse_a->set_last_pos(corpse_a->get_pos());
            corpse_a->Move(vect_force);
            // corpse_a->Move(gmt::Mirrored_Point(temp_pos - corpse_a->get_pos(), vect_force) * damping);

        } else if (!corpse_b->get_fixed()) {
            // corpse_a is Fixed and corpse_b is Not Fixed:
            // gmt::VectorI temp_pos = corpse_b->get_last_pos();

            // corpse_b->set_last_pos(corpse_b->get_pos());
            corpse_b->Move(-vect_force);
            // corpse_b->Move(gmt::Mirrored_Point(temp_pos - corpse_b->get_pos(), vect_force) * damping);
        } else {
            // Both corpse_a and corpse_b are Fixed:
            // corpse_a->Move(force*0.5f);
            // corpse_b->Move(-force*0.5f);
        }
    } else {
        // Both corpse_a and corpse_b are Not Fixed:
        gmt::UnitI normal_mass = corpse_a->get_mass() + corpse_b->get_mass();
        gmt::UnitI normal_mass_corpse_a = corpse_a->get_mass() / normal_mass;
        gmt::UnitI normal_mass_corpse_b = corpse_b->get_mass() / normal_mass;

        // gmt::VectorI diff_pos_a = corpse_a->get_diff_pos();
        // gmt::VectorI diff_pos_b = corpse_b->get_diff_pos();

        // corpse_a->set_last_pos(corpse_a->get_pos());
        // corpse_b->set_last_pos(corpse_b->get_pos());

        corpse_a->Move(vect_force * normal_mass_corpse_b / gmt::UnitI(2));
        corpse_b->Move(-vect_force * normal_mass_corpse_a / gmt::UnitI(2));
        /*
        // Distance between corpses
        gmt::UnitI fDistance = gmt::Length(corpse_a->get_pos_x(), corpse_a->get_pos_y(), corpse_b->get_pos_x(), corpse_b->get_pos_y());

        // Normal
        gmt::UnitI nx = (corpse_b->get_pos_x() - corpse_a->get_pos_x()) / fDistance;
        gmt::UnitI ny = (corpse_b->get_pos_y() - corpse_a->get_pos_y()) / fDistance;

        // Tangent
        gmt::UnitI tx = -nx;
        gmt::UnitI ty = ny;

        // Dot product Tangent
        gmt::UnitI dpTanA = diff_pos_a.x * tx + diff_pos_a.y * ty;
        gmt::UnitI dpTanB = diff_pos_b.x * tx + diff_pos_b.y * ty;

        // Dot product Normal
        gmt::UnitI dpNormA = diff_pos_a.x * nx + diff_pos_a.y * ny;
        gmt::UnitI dpNormB = diff_pos_b.x * nx + diff_pos_b.y * ny;

        // Conservation of momentum
        gmt::UnitI mA = (dpNormA * (corpse_a->get_mass() - corpse_b->get_mass()) + 2.0f * corpse_b->get_mass() * dpNormB) / (corpse_a->get_mass() + corpse_b->get_mass());
        gmt::UnitI mB = (dpNormB * (corpse_b->get_mass() - corpse_a->get_mass()) + 2.0f * corpse_a->get_mass() * dpNormA) / (corpse_a->get_mass() + corpse_b->get_mass());

        corpse_a->Move(tx * dpTanA + nx * mA, ty * dpTanA + ny * mA);
        corpse_b->Move(tx * dpTanB + nx * mB, ty * dpTanB + ny * mB);
        */

        // corpse_a->Move(gmt::Mirrored_Point(temp_pos_a - corpse_a->get_pos(), vect_force) * damping);
        // corpse_b->Move(gmt::Mirrored_Point(temp_pos_b - corpse_b->get_pos(), vect_force) * damping);
    }
}

sf::Color Corpse::get_color() const { return this->color; }
void Corpse::set_color(sf::Color color) { this->color = color; }

gmt::VectorI Corpse::get_pos() const { return this->current_pos; }
gmt::UnitI Corpse::get_pos_x() const { return this->current_pos.x; }
gmt::UnitI Corpse::get_pos_y() const { return this->current_pos.y; }

void Corpse::set_pos(gmt::VectorI pos) { this->current_pos = pos; }
void Corpse::set_pos_x(gmt::UnitI pos_x) { this->current_pos.x = pos_x; }
void Corpse::set_pos_y(gmt::UnitI pos_y) { this->current_pos.y = pos_y; }

gmt::VectorI Corpse::get_last_pos() const { return this->last_pos; }
gmt::UnitI Corpse::get_last_pos_x() const { return this->last_pos.x; }
gmt::UnitI Corpse::get_last_pos_y() const { return this->last_pos.y; }

void Corpse::set_last_pos(gmt::VectorI pos) { this->last_pos = pos; }
void Corpse::set_last_pos_x(gmt::UnitI pos_x) { this->last_pos.x = pos_x; }
void Corpse::set_last_pos_y(gmt::UnitI pos_y) { this->last_pos.y = pos_y; }

gmt::VectorI Corpse::get_diff_pos() const { return (this->get_pos() - this->get_last_pos()); }
gmt::UnitI Corpse::get_diff_pos_x() const { return (this->get_pos_x() - this->get_last_pos_x()); }
gmt::UnitI Corpse::get_diff_pos_y() const { return (this->get_pos_y() - this->get_last_pos_y()); }

gmt::VectorI Corpse::get_propulsor() const { return this->propulsor; };
void Corpse::set_propulsor(gmt::VectorI propulsor) { this->propulsor = propulsor; }

gmt::UnitI Corpse::get_rotation() const { return this->current_rotation; }
void Corpse::set_rotation(gmt::UnitI current_rotation) { this->current_rotation = current_rotation; }
gmt::UnitI Corpse::get_diff_rotation() const { return (this->get_rotation() - this->get_last_rotation()); }

gmt::UnitI Corpse::get_last_rotation() const { return this->last_rotation; }
void Corpse::set_last_rotation(gmt::UnitI last_rotation) { this->last_rotation = last_rotation; }

gmt::UnitI Corpse::get_motor() const { return this->motor; }
void Corpse::set_motor(gmt::UnitI motor) { this->motor = motor; }

void Corpse::set_damping(gmt::UnitI damping) { this->damping = std::min(damping, gmt::UnitI(MIN_DAMPING)); }
gmt::UnitI Corpse::get_damping() const { return this->damping; }
gmt::UnitI Corpse::get_bounce() const { return gmt::UnitI(1) / this->damping; }

gmt::UnitI Corpse::get_mass() const { return this->mass; }
void Corpse::set_mass(gmt::UnitI mass) { this->mass = mass; }

gmt::UnitI Corpse::get_friction() const { return this->friction; }
void Corpse::set_friction(gmt::UnitI friction) { this->friction = friction; }

bool Corpse::Equals(const Corpse* other) { return (this->get_id() == other->get_id()); }

inline bool Corpse::operator==(const Corpse* other) { return this->Equals(other); }

}  // namespace phy