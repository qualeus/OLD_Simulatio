#ifndef Collision_HPP
#define Collision_HPPs

#include "../Corpses/Circle.hpp"
#include "../Corpses/Corpse.hpp"
#include "../Corpses/Polygon.hpp"
#include "Bounds.hpp"
#include "Maths.hpp"
#include "Vector.hpp"
#include "Vertices.hpp"

namespace gmt {

template <typename T>
class Collision {
   private:
    Collision(phy::Corpse* lhs, phy::Corpse* rhs, gmt::VectorI origin, gmt::VectorI normal, bool resolved);
    phy::Corpse* lhs;
    phy::Corpse* rhs;

   public:
    VectorI origin;  // Collision Origin
    VectorI normal;  // Collision Axis
    bool resolved;   // If a collision occured

    static Collision<T> Resolve(std::shared_ptr<phy::Corpse> lhs, std::shared_ptr<phy::Corpse> rhs);
    static Collision<T> CircleOnCircle(phy::Circle* circleA, phy::Circle* circleB);
    static Collision<T> CircleOnPolygon(phy::Circle* circle, phy::Polygon* polygon);
    static Collision<T> PolygonOnPolygon(phy::Polygon* polygonA, phy::Polygon* polygonB);
    static Collision<T> Response(phy::Corpse* lhs, phy::Corpse* rhs, const VectorI& origin, const VectorI& normal);
};
}  // namespace gmt
#endif