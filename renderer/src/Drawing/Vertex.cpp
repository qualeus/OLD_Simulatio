#include "../../include/Drawing/Vertex.hpp"

namespace drw {

Vertex::Vertex(glm::vec3 position, uint32_t color) {
    this->position = position;
    this->color = color;
}

}  // namespace drw