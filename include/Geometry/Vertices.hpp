#ifndef Vertices_HPP
#define Vertices_HPP

#include "Maths.hpp"
#include "Vector.hpp"

namespace gmt {

template <typename T>
class Vertices {
   public:
    std::vector<Vector<T>> vertices;

    Vertices(std::vector<Vector<T>>& vertices);
    Vertices();
    ~Vertices();
    Vector<T> Centroid() const;
    Vector<T> Average() const;
    std::vector<std::pair<Vector<T>, Vector<T>>> Pairs() const;

    /*
    Reorder
    Transformations (Rotate...)
    */

    static std::pair<Vector<T>, Vector<T>> ClosestEdge(const Vertices<T>& vertices, const Vector<T>& point);
};
}  // namespace gmt
#endif