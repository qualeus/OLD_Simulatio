#include "../../include/Drawing/Mesh.hpp"

namespace drw {

Mesh::Mesh() {
    this->vertices = {};
    this->indexes = {};
}

void Mesh::Draw(bgfx::ProgramHandle program) {
    bgfx::VertexLayout declaration;

    declaration
        .begin()                                                      // init
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)      // vertex
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)  // color
        .end();                                                       // stop

    bgfx::VertexBufferHandle v_buffer = bgfx::createVertexBuffer(bgfx::makeRef(std::data(this->vertices), this->vertices.size()), declaration);
    bgfx::IndexBufferHandle i_buffer = bgfx::createIndexBuffer(bgfx::makeRef(std::data(this->indexes), this->indexes.size()));

    bgfx::setVertexBuffer(0, v_buffer);
    bgfx::setIndexBuffer(i_buffer);

    bgfx::submit(0, program);
}

}  // namespace drw