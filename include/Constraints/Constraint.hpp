#ifndef Contraint_HPP
#define Contraint_HPP

#include "../Geometry/Geometry.hpp"
#include "../Geometry/Maths.hpp"
#include "../Corpses/Circle.hpp"
#include "../Corpses/Corpse.hpp"
#include "../Corpses/Polygon.hpp"
#include "Config.hpp"

namespace phy {

const static int ID_CONSTRAINT = 1;

class Constraint {
   protected:
    int id;

    std::shared_ptr<phy::Corpse> corpse_a;
    std::shared_ptr<phy::Corpse> corpse_b;

    gmt::VectorI relative_pos_a;
    gmt::VectorI relative_pos_b;

    bool rotation_a;
    bool rotation_b;

    gmt::UnitI relative_angle_a;
    gmt::UnitI relative_angle_b;

    gmt::UnitI friction_a;
    gmt::UnitI friction_b;

   public:
    explicit Constraint(std::shared_ptr<phy::Corpse> corpse_a, std::shared_ptr<phy::Corpse> corpse_b, gmt::VectorI relative_pos_a , gmt::VectorI relative_pos_b , gmt::UnitI relative_angle_a , gmt::UnitI relative_angle_b, bool rotation_a, bool rotation_b, gmt::UnitI friction_a, gmt::UnitI friction_b);
    inline bool operator==(const Constraint* other);
    Constraint& operator=(const Constraint& rhs);

    int get_id() const;
    virtual int get_class() const;
    static int id_class();

    virtual void Step() = 0;

    bool Equals(const Constraint* other);
    
    std::shared_ptr<phy::Corpse> get_corpse_a();
    void set_corpse_a(std::shared_ptr<phy::Corpse> corpse_a);

    std::shared_ptr<phy::Corpse> get_corpse_b();
    void set_corpse_b(std::shared_ptr<phy::Corpse> corpse_b);

    gmt::VectorI get_relative_pos_a();
    void set_relative_pos_a(gmt::VectorI relative_pos_a);

    gmt::VectorI get_relative_pos_b();
    void set_relative_pos_b(gmt::VectorI relative_pos_b);

    bool get_rotation_a();
    void set_rotation_a(bool rotation_a);

    bool get_rotation_b();
    void set_rotation_b(bool rotation_b);

    gmt::UnitI get_relative_angle_a();
    void set_relative_angle_a(gmt::UnitI angle_a);

    gmt::UnitI get_relative_angle_b();
    void set_relative_angle_b(gmt::UnitI angle_b);

    gmt::UnitI get_friction_a();
    void set_friction_a(gmt::UnitI friction_a);

    gmt::UnitI get_friction_b();
    void set_friction_b(gmt::UnitI friction_b);
};
}  // namespace phy

#endif