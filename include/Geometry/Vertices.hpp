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

    bool Convex() const;
    T Area() const;
    std::vector<T> Sizes() const;
    Bounds<T> Bounds() const;

    Vertices<T> Reorder() const;
    Vertices<T> Rotate(const T& rotation) const;
    Vertices<T> Translate(const Vector<T>& translation) const;
    Vertices<T> Scale(const T& scale) const;

    Vertices<T> Hull() const;
    std::vector<Vertices<T>> Triangulate() const;

    static bool PointInShape(const Vertices<T>& vertices, const Vector<T>& point);
    static bool PointOutShape(const Vertices<T>& vertices, const Vector<T>& point);
    static std::pair<Vector<T>, Vector<T>> ClosestEdge(const Vertices<T>& vertices, const Vector<T>& point);
};
}  // namespace gmt
#endif