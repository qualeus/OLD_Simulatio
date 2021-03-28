#include "../../include/Structures/QuadNode.hpp"

namespace gmt {

/* Vectors Operators: QuadTree=Copy */
template <typename T>
QuadNode<T>& QuadNode<T>::operator=(const QuadNode<T>& rhs) {
    return *this;
}
template QuadNode<int>& QuadNode<int>::operator=(const QuadNode<int>& rhs);
template QuadNode<float>& QuadNode<float>::operator=(const QuadNode<float>& rhs);
template QuadNode<double>& QuadNode<double>::operator=(const QuadNode<double>& rhs);

/* Builder Constructor */
template <typename T>
QuadNode<T>::QuadNode() {
    this->childs = {nullptr, nullptr, nullptr, nullptr};
    this->corpses = {};
}
template QuadNode<int>::QuadNode();
template QuadNode<float>::QuadNode();
template QuadNode<double>::QuadNode();

/* Default Destructor */
/*
template <typename T>
QuadNode<T>::~QuadNode() {}
template QuadNode<int>::~QuadNode();
template QuadNode<float>::~QuadNode();
template QuadNode<double>::~QuadNode();
*/

template <typename T>
bool QuadNode<T>::Leaf() {
    return !static_cast<bool>(this->childs[0]);
}
template bool QuadNode<int>::Leaf();
template bool QuadNode<float>::Leaf();
template bool QuadNode<double>::Leaf();

}  // namespace gmt