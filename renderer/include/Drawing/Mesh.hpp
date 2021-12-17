#ifndef Shader_HPP
#define Shader_HPP

#include <bgfx/bgfx.h>

#include <glm/vec3.hpp>

namespace drw {

// Triangle mesh
class Mesh {
   public:
    std::vector<drw::Vertex> vertices;
    std::vector<drw::Vertex> indexes;

    Mesh();
    bgfx::VertexLayout Convert();
};

}  // namespace drw
#endif