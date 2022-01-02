#ifndef Mesh_HPP
#define Mesh_HPP

#include <bgfx/bgfx.h>

#include <Geometry/String.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "Vertex.hpp"

namespace drw {

// Triangle mesh
class Mesh {
   public:
    std::vector<Vertex<float>> vertices;
    std::vector<uint32_t> indexes;

    Mesh();
};

}  // namespace drw
#endif