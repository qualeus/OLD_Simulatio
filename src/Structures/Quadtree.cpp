#include "../../include/Structures/Quadtree.hpp"

namespace gmt {

/* Vectors Operators: Quadtree=Copy */
template <typename T>
Quadtree<T>& Quadtree<T>::operator=(const Quadtree<T>& rhs) {
    return *this;
}
template Quadtree<int>& Quadtree<int>::operator=(const Quadtree<int>& rhs);
template Quadtree<float>& Quadtree<float>::operator=(const Quadtree<float>& rhs);
template Quadtree<double>& Quadtree<double>::operator=(const Quadtree<double>& rhs);

/* Builder Constructor */
template <typename T>
Quadtree<T>::Quadtree(const T& width, const T& height, const int& max_elems, const int& max_depth) {
    this->width = width;
    this->height = height;
    this->max_elems = max_elems;
    this->max_depth = max_depth;
}
template Quadtree<int>::Quadtree(const int& width, const int& height, const int& max_elems, const int& max_depth);
template Quadtree<float>::Quadtree(const float& width, const float& height, const int& max_elems, const int& max_depth);
template Quadtree<double>::Quadtree(const double& width, const double& height, const int& max_elems, const int& max_depth);

/* Default Destructor */
template <typename T>
Quadtree<T>::~Quadtree() {}
template Quadtree<int>::~Quadtree();
template Quadtree<float>::~Quadtree();
template Quadtree<double>::~Quadtree();

}  // namespace gmt