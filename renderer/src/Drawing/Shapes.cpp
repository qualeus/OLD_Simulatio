#include "../../include/Drawing/Shapes.hpp"

namespace drw {

uint32_t Shapes::triangles = 0;
uint32_t Shapes::rectangles = 0;
uint32_t Shapes::lines = 0;
uint32_t Shapes::arrows = 0;
uint32_t Shapes::circles = 0;
uint32_t Shapes::springs = 0;
uint32_t Shapes::polygons = 0;

Shapes::Shapes() {}

void Shapes::Reset() {
    Shapes::triangles = 0;
    Shapes::rectangles = 0;
    Shapes::lines = 0;
    Shapes::arrows = 0;
    Shapes::circles = 0;
    Shapes::springs = 0;
    Shapes::polygons = 0;
}

void Shapes::Draw(const Mesh& mesh, const bgfx::ProgramHandle& program) {
    bgfx::VertexLayout declaration;

    declaration.begin();                                                      // init
    declaration.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);      // vertex
    declaration.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);  // color
    declaration.end();                                                        // stop

    uint32_t maxVertices = BUFFER_SIZE;
    // uint32_t maxIndexes = BUFFER_SIZE;

    bgfx::TransientVertexBuffer tvb;
    // bgfx::TransientIndexBuffer tib;

    bgfx::allocTransientVertexBuffer(&tvb, maxVertices, declaration);
    // bgfx::allocTransientIndexBuffer(&tib, maxVertices);

    Vertex<float>* tvb_ptr = (Vertex<float>*)tvb.data;
    // uint32_t* tib_ptr = (uint32_t*)tib.data;

    if (mesh.vertices.size() > maxVertices) LOG_ERROR("No more available space in Transient Vertex Buffer");
    // if (mesh.indexes.size() > maxIndexes) LOG_ERROR("No more available space in Transient Index Buffer");

    std::copy(mesh.vertices.begin(), mesh.vertices.end(), tvb_ptr);
    // std::copy(mesh.indexes.begin(), mesh.indexes.end(), tib_ptr);

    bgfx::setVertexBuffer(0, &tvb, 0, mesh.vertices.size());
    // bgfx::setIndexBuffer(&tib, 0, mesh.indexes.size()); // TODO: fon't work, investigte...

    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::submit(0, program);
}

void Shapes::DrawTriangle(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3, uint32_t color) {
    const int vertices_size = mesh.vertices.size();

    mesh.vertices.push_back(Vertex<float>(glm::vec3(pt1.x, pt1.y, pt1.z), color));
    mesh.vertices.push_back(Vertex<float>(glm::vec3(pt2.x, pt2.y, pt2.z), color));
    mesh.vertices.push_back(Vertex<float>(glm::vec3(pt3.x, pt3.y, pt3.z), color));

    mesh.indexes.push_back(vertices_size + 0);
    mesh.indexes.push_back(vertices_size + 1);
    mesh.indexes.push_back(vertices_size + 2);

    Shapes::triangles++;
}

void Shapes::DrawPlane(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3, const glm::vec3& pt4, uint32_t color) {
    DrawTriangle(mesh, pt1, pt2, pt3, color);
    DrawTriangle(mesh, pt1, pt3, pt4, color);

    Shapes::rectangles++;
}

void Shapes::DrawQuad(Mesh& mesh, const glm::vec3& center, const float& radius, uint32_t color) {
    // TODO: normals with screen orientation
    const glm::vec3 pt1 = glm::vec3(center.x - radius, center.y - radius, center.z);
    const glm::vec3 pt2 = glm::vec3(center.x - radius, center.y + radius, center.z);
    const glm::vec3 pt3 = glm::vec3(center.x + radius, center.y + radius, center.z);
    const glm::vec3 pt4 = glm::vec3(center.x + radius, center.y - radius, center.z);

    DrawPlane(mesh, pt1, pt2, pt3, pt4, color);
}

void Shapes::DrawLine(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, uint32_t color) {
    // TODO: normals with screen orientation
    const glm::vec3 vector = pt2 - pt1;
    const glm::vec3 normal = glm::vec3(-vector.y, vector.x, vector.z);
    const glm::vec3 normalized = glm::normalize(normal) * (thickness / 2.0f);

    DrawPlane(mesh, pt1 - normalized, pt1 + normalized, pt2 + normalized, pt2 - normalized, color);

    Shapes::lines++;
}

void Shapes::DrawSpring(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, const float& width, const int& number_waves, uint32_t color) {
    // TODO: normals with screen orientation
    const float inv = 0.25f / number_waves;
    const glm::vec3 diff = (pt2 - pt1) * inv;

    const float inv2 = width / std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    const glm::vec3 diffpn2 = diff * inv2;
    const glm::vec3 diff2 = glm::vec3(-diffpn2.y, diffpn2.x, diffpn2.z);

    glm::vec3 bpt = pt1;

    for (int i = 0; i < number_waves; i++) {
        DrawLine(mesh, bpt, bpt + diff + diff2, thickness, color);
        DrawLine(mesh, bpt + diff + diff2, bpt + 3.0f * diff - diff2, thickness, color);
        DrawLine(mesh, bpt + 3.0f * diff - diff2, bpt + 4.0f * diff, thickness, color);

        bpt += 4.0f * diff;
    }

    Shapes::springs++;
}

void Shapes::DrawArrow(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, const float& sz_head, uint32_t color) {
    // TODO: normals with screen orientation
    const float angle = glm::angle(glm::normalize(pt2), glm::normalize(pt1));
    const float length = glm::distance(pt1, pt2);

    const glm::vec3 bottom = pt1;
    const glm::vec3 head = pt2;
    const glm::vec3 diff = head - bottom;
    const glm::vec3 normal = glm::vec3(-diff.y, diff.x, diff.z);
    const glm::vec3 normalized = glm::normalize(normal);

    const float ratio = sz_head / glm::distance(bottom, head);

    const glm::vec3 head_basis = bottom + diff * (1 - ratio);

    const glm::vec3 basis_A = head_basis + normalized * (sz_head / 2.0f);
    const glm::vec3 basis_B = head_basis - normalized * (sz_head / 2.0f);

    DrawTriangle(mesh, head, basis_A, basis_B, color);
    DrawLine(mesh, bottom, head_basis, thickness, color);

    Shapes::arrows++;
}

void Shapes::DrawRectangle(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, uint32_t color) {
    const glm::vec3 diff = pt2 - pt1;
    const glm::vec3 diffx = glm::vec3(diff.x, 0, 0);
    const glm::vec3 diffy = glm::vec3(0, diff.y, 0);
    const glm::vec3 diffz = glm::vec3(0, 0, diff.z);

    const glm::vec3 spt1 = pt1;
    const glm::vec3 spt2 = pt1 + diffx + diffz;
    const glm::vec3 spt3 = pt1 + diffx + diffy + diffz;
    const glm::vec3 spt4 = pt1 + diffy;

    DrawPlane(mesh, spt1, spt2, spt3, spt4, color);

    Shapes::rectangles++;
}

void Shapes::DrawRectangleOutlined(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, uint32_t color) {
    const glm::vec3 diff = pt2 - pt1;
    const glm::vec3 diffx = glm::vec3(diff.x, 0, 0);
    const glm::vec3 diffy = glm::vec3(0, diff.y, 0);
    const glm::vec3 diffz = glm::vec3(0, 0, diff.z);

    const glm::vec3 spt1 = pt1;
    const glm::vec3 spt2 = pt1 + diffx + diffz;
    const glm::vec3 spt3 = pt1 + diffx + diffy + diffz;
    const glm::vec3 spt4 = pt1 + diffy;

    DrawLine(mesh, spt1, spt2, thickness, color);
    DrawLine(mesh, spt2, spt3, thickness, color);
    DrawLine(mesh, spt3, spt4, thickness, color);
    DrawLine(mesh, spt4, spt1, thickness, color);

    Shapes::rectangles++;
}

void Shapes::DrawPolygon(Mesh& mesh, const std::vector<glm::vec3>& points, uint32_t color) {
    for (int i = 2; i < points.size(); i++) { DrawTriangle(mesh, points[0], points[i - 1], points[i], color); }

    Shapes::polygons++;
}

void Shapes::DrawPolygonOutlined(Mesh& mesh, const std::vector<glm::vec3>& points, const float& thickness, uint32_t color) {
    for (int i = 1; i < points.size(); i++) { DrawLine(mesh, points[i - 1], points[i], thickness, color); }
    DrawLine(mesh, points[0], points[points.size() - 1], thickness, color);

    Shapes::polygons++;
}

/*
void Shapes::DrawCircleFan(int x, int y, int radius, sf::Color color, bool outline) {
    // Using a triangle fan to draw the circle shape. It's quite inneficient, better use a shader
    float cx = static_cast<float>(x);
    float cy = static_cast<float>(y);
    float frad = static_cast<float>(radius);
    int circle resolution = 100;

    gmt::Vector<float> pP = gmt::Vector<float>(cx + frad, cy);

    for (int i = 1; i < circle_resolution; i++) {
        float angle = static_cast<float>(i) * 2.0f * PI / static_cast<float>(circle_resolution - 1);
        gmt::Vector<float> pN = gmt::Vector<float>(cx + std::cos(angle) * frad, cy + std::sin(angle) * frad);

        if (outline) {
            this->DrawLine(pP.x, pP.y, pN.x, pN.y, outline_thickness, color);
        } else {
            this->DrawTriangle(pP.x, pP.y, pN.x, pN.y, cx, cy, color);
        }
        pP = pN;
    }
}

void Shapes::DrawCircle(int x, int y, int radius, sf::Color color, bool outline) {
    if (!gmt::Bounds<float>::CircleIntersectBounds(radius, gmt::Vector<float>(x, y), get_screen_bounds())) { return; }

    if (outline) {
        DrawQuad(x, y, radius * (1.0f + static_cast<float>(OUTLINE_RES)), color, this->outlines_buffer);
    } else {
        DrawQuad(x, y, radius, color, this->circles_buffer);
    }
    this->circles++;
}
*/

}  // namespace drw