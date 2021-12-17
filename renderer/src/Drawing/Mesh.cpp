
namespace drw {

Mesh::Mesh() {
    this->vertices = {};
    this->indexes = {};
}

bgfx::VertexLayout Mesh::Convert() {
    bgfx::VertexDecl declaration;

    declaration
        .begin()                                                      // init
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)      // vertex
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)  // colors
        .end();                                                       // stop

    bgfx::VertexBufferHandle v_buffer = bgfx::createVertexBuffer(bgfx::makeRef(this->vertices, sizeof(this->vertices)), declaration);
    bgfx::IndexBufferHandle i_buffer = bgfx::createIndexBuffer(bgfx::makeRef(this->indexes, sizeof(this->indexes)));
}

}  // namespace drw