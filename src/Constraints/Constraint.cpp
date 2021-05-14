#include "../../include/Constraints/Constraint.hpp"
namespace phy {
Constraint::Constraint(std::shared_ptr<phy::Corpse> corpse_a, std::shared_ptr<phy::Corpse> corpse_b, gmt::VectorI relative_pos_a, gmt::VectorI relative_pos_b, gmt::UnitI relative_angle_a, gmt::UnitI relative_angle_b, bool rotation_a, bool rotation_b, gmt::UnitI friction_a, gmt::UnitI friction_b,
                       bool breaking) {
    static int global_constraint_id = 0;
    this->id = global_constraint_id++;

    this->corpse_a = corpse_a;
    this->corpse_b = corpse_b;

    this->relative_pos_a = relative_pos_a;
    this->relative_pos_b = relative_pos_b;

    this->relative_angle_a = relative_angle_a;
    this->relative_angle_b = relative_angle_b;

    this->rotation_a = rotation_a;
    this->rotation_b = rotation_b;

    this->friction_a = friction_a;
    this->friction_b = friction_b;

    this->breaking = breaking;
    this->broken = false;
}

int Constraint::get_id() const { return this->id; }
int Constraint::get_class() const { return ID_CONSTRAINT; }
static int id_class() { return ID_CONSTRAINT; }

bool Constraint::Equals(const Constraint* other) { return this->get_id() == other->get_id(); }
inline bool Constraint::operator==(const Constraint* other) { return this->Equals(other); }

std::shared_ptr<phy::Corpse> Constraint::get_corpse_a() { return this->corpse_a; }
void Constraint::set_corpse_a(std::shared_ptr<phy::Corpse> corpse_a) { this->corpse_a = corpse_a; }

std::shared_ptr<phy::Corpse> Constraint::get_corpse_b() { return this->corpse_b; }
void Constraint::set_corpse_b(std::shared_ptr<phy::Corpse> corpse_b) { this->corpse_b = corpse_b; }

gmt::VectorI Constraint::get_relative_pos_a() { return this->relative_pos_a; }
void Constraint::set_relative_pos_a(gmt::VectorI relative_pos_a) { this->relative_pos_a = relative_pos_a; }

gmt::VectorI Constraint::get_relative_pos_b() { return this->relative_pos_b; }
void Constraint::set_relative_pos_b(gmt::VectorI relative_pos_b) { this->relative_pos_b = relative_pos_b; }

bool Constraint::get_rotation_a() { return this->rotation_a; }
void Constraint::set_rotation_a(bool rotation_a) { this->rotation_a = rotation_a; }

bool Constraint::get_rotation_b() { return this->rotation_b; }
void Constraint::set_rotation_b(bool rotation_b) { this->rotation_b = rotation_b; }

gmt::UnitI Constraint::get_relative_angle_a() { return this->relative_angle_a; }
void Constraint::set_relative_angle_a(gmt::UnitI relative_angle_a) { this->relative_angle_a = relative_angle_a; }

gmt::UnitI Constraint::get_relative_angle_b() { return this->relative_angle_b; }
void Constraint::set_relative_angle_b(gmt::UnitI relative_angle_b) { this->relative_angle_b = relative_angle_b; }

gmt::UnitI Constraint::get_friction_a() { return this->friction_a; }
void Constraint::set_friction_a(gmt::UnitI friction_a) { this->friction_a = friction_a; }

gmt::UnitI Constraint::get_friction_b() { return this->friction_b; }
void Constraint::set_friction_b(gmt::UnitI friction_b) { this->friction_a = friction_b; }

bool Constraint::get_breaking() { return this->breaking; }
void Constraint::set_breaking(bool breaking) { this->breaking = breaking; }

bool Constraint::get_broken() { return this->broken; }
void Constraint::set_broken(bool broken) { this->broken = broken; }
}  // namespace phy