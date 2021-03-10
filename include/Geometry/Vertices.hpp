#ifndef Vertices_HPP
#define Vertices_HPP

#include "Maths.hpp"

namespace gmt {

template <typename T>
class Vertices {
   public:
    std::vector<std::shared_ptr<Vector<T>>> vertices;

    Vertices(std::vector<std::shared_ptr<Vector<T>>> vertices);
    Vertices();
    ~Vertices();
    Vector<T> Centroid() const;
    Vector<T> Average() const;
    std::vector<std::pair<std::shared_ptr<Vector<T>>, std::shared_ptr<Vector<T>>>> Pairs() const;

    bool Convex() const;
    T Area() const;
    std::vector<T> Sizes() const;
    Bounds<T> Bounds() const;

    Vertices<T> Reorder() const;
    Vertices<T> Translate(const Vector<T>& translation) const;
    Vertices<T> Rotate(const T& rotation, const Vector<T>& centroid) const;
    Vertices<T> Scale(const T& scale, const Vector<T>& centroid) const;

    Vertices<T> Hull() const;
    std::vector<Vertices<T>> Triangulate() const;

    static bool PointInShape(const Vertices<T>& vertices, const Vector<T>& point);
    static bool PointOutShape(const Vertices<T>& vertices, const Vector<T>& point);
    static std::pair<std::shared_ptr<Vector<T>>, std::shared_ptr<Vector<T>>> ClosestEdge(const Vertices<T>& vertices, const Vector<T>& point);
};
}  // namespace gmt
#endif