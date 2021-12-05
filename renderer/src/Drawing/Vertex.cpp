#include "../../include/Drawing/Vertex.hpp"

namespace drw {

Vertex::Vertex(glm::vec3& position, glm::vec4& color) {
    this->position = position;
    this->color = color;
}

void Vertex::set_position(const glm::vec3& position) { this->position = position; }
void Vertex::set_color(const glm::vec4& color) { this->color = color; }

}  // namespace drw
