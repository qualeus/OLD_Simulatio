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

/* Check if the Polygon is Convex */
template <typename T>
bool Vertices<T>::Convex() const {
    if (sides.size() <= 3) { return true; } /* triangles are always convex */

    /* We check for every edges that don't have an edge in common (opposites) if they intersect */
    std::vector<std::pair<Vector<T>, Vector<T>>> sides = this->Pairs();
    for (int i = 0; i < sides.size() - 2; i++) {
        for (int j = i + 2; j < sides.size() - (i == 0); j++) {
            std::pair<VectorI, VectorI> sideA = sides.at(i);
            std::pair<VectorI, VectorI> sideB = sides.at(j);
            if (Segments_Intersect(sideA.first, sideA.second, sideB.first, sideB.second)) { return false; }
        }
    }

    /* We test for each point if the angle is oriented in the same direction*/
    bool orientation = (Vector<T>::Cross(points.at(points.size() - 1) - points.at(0), points.at(1) - points.at(0))) > T(0));
    for (int i = 0; i < points.size() - 2; i++) {
        if ((Vector<T>::Cross(points.at(i) - points.at(i + 1), points.at(i + 2) - points.at(i + 1))) > T(0)) != orientation) { return false; }
    }
    return true;
}
template bool Vertices<int>::Convex() const;
template bool Vertices<float>::Convex() const;
template bool Vertices<double>::Convex() const;

/* Compute the area of the polygon with its edges */
/*
    X = SUM[(Xi * Yi+1 - Xi+1 * Yi)] / 2
    Y = SUM[(Xi * Yi+1 - Xi+1 * Yi)] / 2
*/
template <typename T>
T Vertices<T>::Area() const {
    T signed_area = T(0);
    T A = T(0);
    int vertices_size = this->vertices.size();
    for (int i = 0; i < vertices_size; i++) {
        Vector<T> pA = this->vertices.at(i);
        Vector<T> pB = this->vertices.at((i + 1) % vertices_size);
        A = pA.x * pB.y - pB.x * pA.y;
        signed_area = signed_area + A;
    }
    return signed_area / T(2);
}
template int Vertices<int>::Area() const;
template float Vertices<float>::Area() const;
template double Vertices<double>::Area() const;

/* Return the array for the sizes of the sides */
template <typename T>
std::vector<T> Vertices<T>::Sizes() const {
    std::vector<T> sizes = {};
    int vertices_size = this->vertices.size();
    for (int i = 0; i < vertices_size; i++) {
        Vector<T> pA = this->vertices.at(i);
        Vector<T> pB = this->vertices.at((i + 1) % vertices_size);
        sizes.push_back(Vecror<T>::Distance(pA, pB));
    }
    return sizes;
}
template std::vector<int> Vertices<int>::Sizes() const;
template std::vector<float> Vertices<float>::Sizes() const;
template std::vector<double> Vertices<double>::Sizes() const;

/* Return the bounds of the polygon */
template <typename T>
Bounds<T> Vertices<T>::Bounds() const {
    const auto min_max_x = std::minmax_element(vertices.begin(), vertices.end(), [](const VectorI& lhs, const VectorI& rhs) { return lhs.x < rhs.x; });
    const auto min_max_y = std::minmax_element(vertices.begin(), vertices.end(), [](const VectorI& lhs, const VectorI& rhs) { return lhs.y < rhs.y; });

    Vector<T> min_x = (*min_max_x.first).x;
    Vector<T> max_x = (*min_max_x.second).x;
    Vector<T> min_y = (*min_max_y.first).y;
    Vector<T> max_y = (*min_max_y.second).y;

    return Bounds<T>(min_x, min_y, max_x, max_y);
}
template Bounds<int> Vertices<int>::Bounds() const;
template Bounds<float> Vertices<float>::Bounds() const;
template Bounds<double> Vertices<double>::Bounds() const;

template <typename T>
Vertices<T> Vertices<T>::Reorder() const {
    // TODO
    return this->vertices;
}
template Vertices<int> Vertices<int>::Reorder() const;
template Vertices<float> Vertices<float>::Reorder() const;
template Vertices<double> Vertices<double>::Reorder() const;

/* Rotate the points of the Poilygon from the Centroid */
template <typename T>
Vertices<T> Vertices<T>::Rotate(const T& rotation) const {
    Vector<T> centroid = this->Centroid();
    std::vector<Vector<T>> temp = {};
    for (int i = 0; i < this->vertices.size(); i++) {
        Vector<T> diff = this->vertices.at(i) - centroid;
        temp.push_back(centroid + diff.Rotate(rotation));
    }
    return temp;
}
template Vertices<int> Vertices<int>::Rotate(const int& rotation) const;
template Vertices<float> Vertices<float>::Rotate(const float& rotation) const;
template Vertices<double> Vertices<double>::Rotate(const double& rotation) const;

/* Translate the points of the Poilygon with a translation vector */
template <typename T>
Vertices<T> Vertices<T>::Translate(const Vector<T>& translation) const {
    std::vector<Vector<T>> temp = {};
    for (int i = 0; i < this->vertices.size(); i++) { temp.push_back(this->vertices.at(i) + translation); }
    return temp;
}
template Vertices<int> Vertices<int>::Translate(const Vector<int>& rotation) const;
template Vertices<float> Vertices<float>::Translate(const Vector<float>& rotation) const;
template Vertices<double> Vertices<double>::Translate(const Vector<double>& rotation) const;

/* Scale the size of the polygon from the Centroid */
template <typename T>
Vertices<T> Vertices<T>::Scale(const T& scale) const {
    Vector<T> centroid = this->Centroid();
    std::vector<Vector<T>> temp = {};
    for (int i = 0; i < this->vertices.size(); i++) {
        Vector<T> diff = this->vertices.at(i) - centroid;
        temp.push_back(centroid + diff * scale);
    }
    return temp;
}
template Vertices<int> Vertices<int>::Scale(const int& scale) const;
template Vertices<float> Vertices<float>::Scale(const float& scale) const;
template Vertices<double> Vertices<double>::Scale(const double& scale) const;

/* Convex Hull */
template <typename T>
Vertices<T> Vertices<T>::Hull() const {
    // TODO
    return this->vertices;
}
template Vertices<int> Vertices<int>::Hull() const;
template Vertices<float> Vertices<float>::Hull() const;
template Vertices<double> Vertices<double>::Hull() const;

/* Ear Clipping */
template <typename T>
std::vector<Vertices<T>> Vertices<T>::Triangulate() const {
    // TODO
    return std::vector<Vertices<T>>();
}
template std::vector<Vertices<int>> Vertices<int>::Triangulate() const;
template std::vector<Vertices<float>> Vertices<float>::Triangulate() const;
template std::vector<Vertices<double>> Vertices<double>::Triangulate() const;

/* Return true if the point is contained in the shape */
template <typename T>
bool Vertices<T>::PointInShape(const Vertices<T>& vertices, const Vector<T>& point) {
    /*
    // Ray Casting Method: The ray lenght is the maximum of the sizes of the sides
    std::vector<T> sizes = vertices->Sizes();
    T ray_lenght = *std::max_element(std::begin(sizes), std::end(sizes));  // TODO (bounds diagonal)
    Vector<T> pointB = Vector<T>(v1.x + ray_lenght, v1.y);

    int intersections = 0;
    int vertices_size = vertices->vertices.size();
    for (int i = 0; i < vertices_size; i++) {
        Vector<T> pA = vertices->vertices.at(i);
        Vector<T> pB = vertices->vertices.at((i + 1) % vertices_size);
        if (VectorI::SegmentsIntersect(point, pointB, pA, pB)) { intersections++; }
    }
    if ((intersections & 1) == 1) { return true; }
    return false;
    */

    // TODO test?
    int vertices_size = vertices->vertices.size();
    for (int i = 0; i < vertices_size; i++) {
        Vector<T> pA = vertices->vertices.at(i);
        Vector<T> pB = vertices->vertices.at((i + 1) % vertices_size);
        if ((point.x - pA.x) * (pB.y - pa.y) + (point.y - pA.y) * (pA.x - pB.x) > 0) { return false; }
    }
    return true;
}
template bool Vertices<int>::PointInShape(const Vertices<int>& vertices, const Vector<int>& point);
template bool Vertices<float>::PointInShape(const Vertices<float>& vertices, const Vector<float>& point);
template bool Vertices<double>::PointInShape(const Vertices<double>& vertices, const Vector<double>& point);

/* Return true if the point is not in the shape */
template <typename T>
bool Vertices<T>::PointOutShape(const Vertices<T>& vertices, const Vector<T>& point) {
    return !Vertices<T>::PointInShape(v1);
}
template bool Vertices<int>::PointOutShape(const Vertices<int>& vertices, const Vector<int>& point);
template bool Vertices<float>::PointOutShape(const Vertices<float>& vertices, const Vector<float>& point);
template bool Vertices<double>::PointOutShape(const Vertices<double>& vertices, const Vector<double>& point);

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