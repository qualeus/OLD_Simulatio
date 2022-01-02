#ifndef Vertex_HPP
#define Vertex_HPP

#include <bgfx/bgfx.h>

#include <glm/vec3.hpp>
#include <sstream>
#include <string>

namespace drw {

template <typename T>
class Vertex {
   public:
    glm::tvec3<T> position;
    uint32_t color;

    Vertex(glm::tvec3<T> position, uint32_t color);
    Vertex();
};

}  // namespace drw

#endif