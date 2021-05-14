#include "../../include/Constraints/Link.hpp"

namespace phy {

Link::Link(std::shared_ptr<phy::Corpse> corpse_a, std::shared_ptr<phy::Corpse> corpse_b, gmt::VectorI relative_pos_a , gmt::VectorI relative_pos_b , gmt::UnitI relative_angle_a , gmt::UnitI relative_angle_b, bool rotation_a, bool rotation_b, gmt::UnitI friction_a, gmt::UnitI friction_b, gmt::UnitI size, gmt::UnitI damping, gmt::UnitI max_size, gmt::UnitI min_size, bool breaking) : Constraint(corpse_a, corpse_b, relative_pos_a , relative_pos_b , relative_angle_a , relative_angle_b, rotation_a, rotation_b, friction_a, friction_b) {
    if (size < gmt::UnitI()) {
        this->size = gmt::VectorI::Distance(corpse_a->get_pos(), corpse_b->get_pos());
    } else { 
        this->size = size; 
    }

    this->damping = damping;
    this->max_size = max_size;
    this->min_size = min_size;
    this->breaking = breaking;
    this->broken = false;
}
// Link& Link::operator=(const Link& rhs) {}

int Link::get_class() const { return ID_LINK; }
static int id_class() { return ID_LINK; }

void Link::Step() {
    gmt::VectorI diff = get_corpse_a()->get_pos() - get_corpse_b()->get_pos();
    gmt::UnitI current_size = std::sqrt(gmt::VectorI::Dot(diff, diff));

    gmt::UnitI percent = (current_size - size) / (gmt::UnitI(2) * current_size); 
    gmt::VectorI displacement = diff * percent;

    // TODO Apply in function of mass, fixed etc...
    
    //corpse_a->Drag(displacement);
    //corpse_b->Drag(-displacement);

    /*
    if (current_size > max_size) {
        if (breaking) { this->broken = true; }
        else {}
    } else if (current_size < min_size) {
        if (breaking) { this->broken = true; }
        else {}
    } else {}
    */
}

gmt::UnitI Link::get_size() { return this->size; }
void Link::set_size(gmt::UnitI size) { this->size = size;  }

gmt::UnitI Link::get_damping() { return this->damping; }
void Link::set_damping(gmt::UnitI damping) { this->damping = damping; }

gmt::UnitI Link::get_max_size() { return this->max_size; }
void Link::set_max_size(gmt::UnitI max_size) { this->max_size = max_size; }

gmt::UnitI Link::get_min_size() { return this->min_size; }
void Link::set_min_size(gmt::UnitI min_size) { this->min_size = min_size; }

bool Link::get_breaking() { return this->breaking; }
void Link::set_breaking(bool breaking) { this->breaking = breaking; }

bool Link::get_broken() { return this->broken; }
void Link::set_broken(bool broken) { this->broken = broken; }

}