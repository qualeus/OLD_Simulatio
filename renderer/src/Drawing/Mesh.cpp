#include "../../include/Drawing/Mesh.hpp"

namespace drw {

MeshData::MeshData(RawVertices vertices, RawIndices indices) {
    this->vertices = vertices;
    this->indices = indices;
}

}  // namespace drw