#ifndef Quadtree_HPP
#define Quadtree_HPP

#include "../Corpses/Circle.hpp"
#include "../Corpses/Corpse.hpp"
#include "../Corpses/Polygon.hpp"
#include "../Geometry/Bounds.hpp"
#include "../Geometry/Maths.hpp"
#include "../Geometry/Vector.hpp"

namespace gmt {

template <typename T>
class Quadtree {
   private:
   public:
    Quadtree<T>& operator=(const Quadtree<T>& rhs);

    Quadtree();
    ~Quadtree();
};

}  // namespace gmt

#endif