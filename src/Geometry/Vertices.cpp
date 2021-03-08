#include "../../include/Geometry/Vertices.hpp"

namespace gmt {

/* Builder Constructor */
template <typename T>
Vertices<T>::Vertices(std::vector<Vector<T>>& vertices) {
    this->vertices = vertices;
}
template Vertices<int>::Vertices(std::vector<Vector<int>>& vertices);
template Vertices<float>::Vertices(std::vector<Vector<float>>& vertices);
template Vertices<double>::Vertices(std::vector<Vector<double>>& vertices);

/* Default Constructor */
template <typename T>
Vertices<T>::Vertices() {
    this->vertices = {};
}
template Vertices<int>::Vertices();
template Vertices<float>::Vertices();
template Vertices<double>::Vertices();

/* Default Destructor */
template <typename T>
Vertices<T>::~Vertices() {}
template Vertices<int>::~Vertices();
template Vertices<float>::~Vertices();
template Vertices<double>::~Vertices();

/* Compute the centroid of the polygon with its edges */
/*
    X = SUM[(Xi + Xi+1) * (Xi * Yi+1 - Xi+1 * Yi)] / 6 / A
    Y = SUM[(Yi + Yi+1) * (Xi * Yi+1 - Xi+1 * Yi)] / 6 / A
*/
template <typename T>
Vector<T> Vertices<T>::Centroid() const {
    Vector<T> centroid = Vector<T>();
    T signed_area = T(0);
    T A = T(0);
    int vertices_size = this->vertices.size();
    for (int i = 0; i < vertices_size; i++) {
        Vector<T> pA = this->vertices.at(i);
        Vector<T> pB = this->vertices.at((i + 1) % vertices_size);
        A = pA.x * pB.y - pB.x * pA.y;
        signed_area = signed_area + A;
        centroid = centroid + (pA + pB) * A;
    }

    if (signed_area == 0) { return Vector<T>(); }
    signed_area = signed_area / T(2);
    centroid = centroid / static_cast<T>(T(6) * signed_area);
    return centroid;
}
template Vector<int> Vertices<int>::Centroid() const;
template Vector<float> Vertices<float>::Centroid() const;
template Vector<double> Vertices<double>::Centroid() const;

/* Compute the averages of the points */
template <typename T>
Vector<T> Vertices<T>::Average() const {
    Vector<T> points_average = Vector<T>();
    if (this->vertices.size() == 0) { return points_average; }

    for (int i = 0; i < this->vertices.size(); i++) { points_average = points_average + this->vertices.at(i); }
    return points_average / static_cast<T>(this->vertices.size());
}
template Vector<int> Vertices<int>::Average() const;
template Vector<float> Vertices<float>::Average() const;
template Vector<double> Vertices<double>::Average() const;

/* Return the Vectors Pairs in a Clockwise Order */
template <typename T>
std::vector<std::pair<Vector<T>, Vector<T>>> Vertices<T>::Pairs() const {
    std::vector<std::pair<Vector<T>, Vector<T>>> pairs = {};
    int vertices_size = this->vertices.size();
    for (int i = 0; i < vertices_size; i++) {
        Vector<T> pA = this->vertices.at(i);
        Vector<T> pB = this->vertices.at((i + 1) % vertices_size);
        pairs.push_back({pA, pB});
    }
    return pairs;
}
template std::vector<std::pair<Vector<int>, Vector<int>>> Vertices<int>::Pairs() const;
template std::vector<std::pair<Vector<float>, Vector<float>>> Vertices<float>::Pairs() const;
template std::vector<std::pair<Vector<double>, Vector<double>>> Vertices<double>::Pairs() const;

/* Find the closest edge to the point by finding the closest projected point */
template <typename T>
std::pair<Vector<T>, Vector<T>> Vertices<T>::ClosestEdge(const Vertices<T>& vertices, const Vector<T>& point) {
    std::vector<std::pair<Vector<T>, Vector<T>>> pairs = vertices.Pairs();
    const auto closest = std::min_element(pairs.begin(), pairs.end(), [point](const std::pair<Vector<T>, Vector<T>>& lhs, const std::pair<Vector<T>, Vector<T>>& rhs) {
        Vector<T> pro_lhs = Vector<T>::SegmentProjection(point, lhs.first, lhs.second);
        Vector<T> pro_rhs = Vector<T>::SegmentProjection(point, rhs.first, rhs.second);

        T dot_dist_lhs = Vector<T>::Distance(pro_lhs, point);
        T dot_dist_rhs = Vector<T>::Distance(pro_rhs, point);
        return dot_dist_lhs < dot_dist_rhs;
    });
    std::pair<Vector<T>, Vector<T>> closest_side = (*closest);
    return closest_side;
}
template std::pair<Vector<int>, Vector<int>> Vertices<int>::ClosestEdge(const Vertices<int>& vertices, const Vector<int>& point);
template std::pair<Vector<float>, Vector<float>> Vertices<float>::ClosestEdge(const Vertices<float>& vertices, const Vector<float>& point);
template std::pair<Vector<double>, Vector<double>> Vertices<double>::ClosestEdge(const Vertices<double>& vertices, const Vector<double>& point);

}  // namespace gmt