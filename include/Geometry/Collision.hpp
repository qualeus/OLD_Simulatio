#ifndef Collision_HPP
#define Collision_HPPs

#include "Maths.hpp"

namespace gmt {

template <typename T>
class Collision {
   private:
    Collision() {}  // Disallow creating an instance of this object

   public:
    static void Resolve(std::shared_ptr<phy::Corpse> lhs, std::shared_ptr<phy::Corpse> rhs);
    static void CircleOnCircle(phy::Circle* circleA, phy::Circle* circleB);
    static void CircleOnPolygon(phy::Circle* circle, phy::Polygon* polygon);
    static void PolygonOnPolygon(phy::Polygon* polygonA, phy::Polygon* polygonB);
    static void Response(phy::Corpse* lhs, phy::Corpse* rhs, const Vector<T>& normal);
};
}  // namespace gmt
#endif