#ifndef Vertex_HPP
#define Vertex_HPP

#include <glm/glm.hpp>

namespace drw {

class Vertex {
   private:
    glm::vec3 position;
    glm::vec4 color;

   public:
    Vertex(glm::vec3& position, glm::vec4& color);
    void set_position(const glm::vec3& position);
    void set_color(const glm::vec4& color);
};

}  // namespace drw

#endif