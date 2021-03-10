#ifndef Circle_HPP
#define Circle_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Config.hpp"
#include "Corpse.hpp"
#include "Polygon.hpp"

namespace phy {

const static int ID_CIRCLE = 2;

class Circle : public Corpse {
   private:
    gmt::UnitI size;

   public:
    Circle(gmt::UnitI x = gmt::UnitI(0), gmt::UnitI y = gmt::UnitI(0), gmt::UnitI size = gmt::UnitI(1), gmt::UnitI mass = gmt::UnitI(1), gmt::UnitI damping = gmt::UnitI(1), gmt::UnitI speed_x = gmt::UnitI(0), gmt::UnitI speed_y = gmt::UnitI(0), gmt::UnitI rotation = gmt::UnitI(0),
           gmt::UnitI motor = gmt::UnitI(0), gmt::VectorI propulsor = gmt::VectorI(), bool fixed = false, bool tied = false, bool etherial = false, sf::Color color = sf::Color::White);
    virtual ~Circle();
    Circle& operator=(const Circle& rhs);

    int get_class() const;

    void Step();
    void Stop();
    void Move(gmt::UnitI x, gmt::UnitI y, bool relative = true);
    void Move(gmt::VectorI move, bool relative = true);
    bool inBounds(gmt::UnitI x1, gmt::UnitI x2, gmt::UnitI y1, gmt::UnitI y2);
    bool Pointed(gmt::UnitI x, gmt::UnitI y);

    void Collision(std::shared_ptr<Corpse> a);

    gmt::UnitI get_size() const;
    gmt::BoundsI get_corpse_bounds() const;
};

}  // namespace phy

#endif