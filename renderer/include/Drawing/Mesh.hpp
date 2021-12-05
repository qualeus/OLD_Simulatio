#ifndef Mesh_HPP
#define Mesh_HPP

#include <vector>

#include "Vertex.hpp"

namespace drw {

using RawVertices = std::vector<Vertex>;
using RawIndices = std::vector<uint32_t>;

class MeshData {
   private:
    RawVertices vertices;
    RawIndices indices;

   public:
    MeshData(RawVertices vertices, RawIndices indices);
};

}  // namespace drw

#endif