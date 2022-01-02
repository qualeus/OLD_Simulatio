#include "../../include/Drawing/Vertex.hpp"

namespace drw {

template <typename T>
Vertex<T>::Vertex(glm::tvec3<T> position, uint32_t color) {
    this->position = position;
    this->color = color;
}
template Vertex<float>::Vertex(glm::tvec3<float> position, uint32_t color);
template Vertex<int>::Vertex(glm::tvec3<int> position, uint32_t color);

template <typename T>
Vertex<T>::Vertex() {
    this->position = glm::tvec3<T>();
    this->color = 0x00000000;
}
template Vertex<float>::Vertex();
template Vertex<int>::Vertex();

}  // namespace drw