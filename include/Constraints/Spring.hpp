#ifndef Spring_HPP
#define Spring_HPP

#include "Config.hpp"
#include "Constraint.hpp"

namespace phy {

const static int ID_SPRING = 2;

class Spring : public Constraint {
   private:
    gmt::UnitI size;
    gmt::UnitI damping;
    gmt::UnitI max_size;
    gmt::UnitI min_size;

   public:
    Spring(std::shared_ptr<phy::Corpse> corpse_a, std::shared_ptr<phy::Corpse> corpse_b, gmt::VectorI relative_pos_a = gmt::VectorI(), gmt::VectorI relative_pos_b = gmt::VectorI(), gmt::UnitI relative_angle_a = gmt::UnitI(), gmt::UnitI relative_angle_b = gmt::UnitI(), bool rotation_a = true,
           bool rotation_b = true, gmt::UnitI friction_a = gmt::UnitI(), gmt::UnitI friction_b = gmt::UnitI(), gmt::UnitI size = gmt::UnitI(-1), gmt::UnitI damping = gmt::UnitI(1), gmt::UnitI max_size = gmt::UnitI(-1), gmt::UnitI min_size = gmt::UnitI(-1), bool breaking = false);
    Spring& operator=(const Spring& rhs);

    int get_class() const;
    static int id_class();

    void Step();

    gmt::UnitI get_size();
    void set_size(gmt::UnitI size);

    gmt::UnitI get_damping();
    void set_damping(gmt::UnitI damping);

    gmt::UnitI get_max_size();
    void set_max_size(gmt::UnitI max_size);

    gmt::UnitI get_min_size();
    void set_min_size(gmt::UnitI min_size);
};
}  // namespace phy
#endif