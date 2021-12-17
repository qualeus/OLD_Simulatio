#ifndef Shader_HPP
#define Shader_HPP

#include <bgfx/bgfx.h>

#include <glm/vec3.hpp>

namespace drw {

class Vertex {
   public:
    glm::vec3 position;
    uint32_t color;

    Vertex(glm::vec3 position, uint32_t color);
};

}  // namespace drw
#endif