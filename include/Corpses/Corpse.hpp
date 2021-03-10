#ifndef Corpse_HPP
#define Corpse_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "../Geometry/Geometry.hpp"
#include "Config.hpp"

namespace phy {

const static int ID_CORPSE = 1;

class Corpse {
   protected:
    int id;
    bool fixed;
    bool tied;
    bool etherial;
    bool removed;

    gmt::UnitI friction;
    gmt::UnitI mass;
    gmt::UnitI damping;

    gmt::VectorI current_pos;
    gmt::VectorI last_pos;
    gmt::VectorI propulsor;

    gmt::UnitI current_rotation;
    gmt::UnitI last_rotation;
    gmt::UnitI motor;

    sf::Color color;

   public:
    explicit Corpse(gmt::UnitI mass, gmt::UnitI damping, bool fixed, bool tied, bool etherial, sf::Color color);
    virtual ~Corpse();
    inline bool operator==(const Corpse* other);
    Corpse& operator=(const Corpse& rhs);

    int get_id() const;
    virtual int get_class() const;

    bool get_fixed() const;
    void set_fixed(bool fixed);

    bool get_etherial() const;
    void set_etherial(bool etherial);

    bool get_tied() const;
    void set_tied(bool tied);

    bool get_removed() const;
    void Remove();

    virtual void Step();
    virtual void Stop();
    virtual void Move(gmt::UnitI x, gmt::UnitI y, bool relative = true);
    virtual void Move(gmt::VectorI move, bool relative = true);
    virtual bool inBounds(gmt::UnitI x1, gmt::UnitI x2, gmt::UnitI y1, gmt::UnitI y2) = 0;
    virtual bool Pointed(gmt::UnitI x, gmt::UnitI y) = 0;

    virtual void Collision(std::shared_ptr<Corpse> a);

    void CollisionResponse(phy::Corpse* corpse_a, phy::Corpse* corpse_b, const gmt::VectorI& vect_force);

    sf::Color get_color() const;
    void set_color(sf::Color color);

    gmt::VectorI get_pos() const;
    gmt::UnitI get_pos_x() const;
    gmt::UnitI get_pos_y() const;

    void set_pos(gmt::VectorI pos);
    void set_pos_x(gmt::UnitI pos_x);
    void set_pos_y(gmt::UnitI pos_y);

    gmt::VectorI get_last_pos() const;
    gmt::UnitI get_last_pos_x() const;
    gmt::UnitI get_last_pos_y() const;

    void set_last_pos(gmt::VectorI pos);
    void set_last_pos_x(gmt::UnitI pos_x);
    void set_last_pos_y(gmt::UnitI pos_y);

    gmt::VectorI get_diff_pos() const;
    gmt::UnitI get_diff_pos_x() const;
    gmt::UnitI get_diff_pos_y() const;

    gmt::VectorI get_propulsor() const;
    void set_propulsor(gmt::VectorI propulsor);

    gmt::UnitI get_rotation() const;
    void set_rotation(gmt::UnitI current_rotation);
    gmt::UnitI get_diff_rotation() const;

    gmt::UnitI get_last_rotation() const;
    void set_last_rotation(gmt::UnitI last_rotation);

    gmt::UnitI get_motor() const;
    void set_motor(gmt::UnitI motor);

    void set_damping(gmt::UnitI damping);
    gmt::UnitI get_damping() const;
    gmt::UnitI get_bounce() const;

    gmt::UnitI get_mass() const;
    void set_mass(gmt::UnitI mass);

    gmt::UnitI get_friction() const;
    void set_friction(gmt::UnitI friction);

    virtual gmt::BoundsI get_corpse_bounds() const = 0;

    bool Equals(const Corpse* other);
};

}  // namespace phy

#endif