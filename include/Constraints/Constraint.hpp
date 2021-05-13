#ifndef Contraint_HPP
#define Contraint_HPP

#include "../Geometry/Geometry.hpp"
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

    gmt::UnitI angle_a;
    gmt::UnitI angle_b;

    gmt::UnitI friciton_a;
    gmt::UnitI friciton_b;

   public:
    explicit Constraint();
    inline bool operator==(const Constraint* other);
    Constraint& operator=(const Constraint& rhs);

    int get_id() const;
    virtual int get_class() const;
    static int id_class();

    virtual void Step();

    bool Equals(const Constraint* other);
};
}  // namespace phy

#endif