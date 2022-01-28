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

    uint32_t maxVertices = 300000;
    uint32_t maxIndexes = 300000;

    bgfx::TransientVertexBuffer tvb;
    bgfx::TransientIndexBuffer tib;

    bgfx::allocTransientVertexBuffer(&tvb, maxVertices, declaration);
    bgfx::allocTransientIndexBuffer(&tib, maxVertices);

    Vertex<float>* tvb_ptr = (Vertex<float>*)tvb.data;
    uint32_t* tib_ptr = (uint32_t*)tib.data;

    if (mesh.vertices.size() > maxVertices) LOG_ERROR("No more available space in Transient Vertex Buffer");
    if (mesh.indexes.size() > maxIndexes) LOG_ERROR("No more available space in Transient Index Buffer");

    std::copy(mesh.vertices.begin(), mesh.vertices.end(), tvb_ptr);
    std::copy(mesh.indexes.begin(), mesh.indexes.end(), tib_ptr);

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

/*
void Shapes::DrawArrow(int x1, int y1, int x2, int y2, int xhead, int yhead, int thickness, sf::Color color) {
    if (!gmt::Bounds<float>::SegmentIntersectBounds(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2), get_screen_bounds())) { return; }

    float angle = gmt::Vector<float>::Bearing(gmt::Vector<float>(x2, y2), gmt::Vector<float>(x1, y1));
    float length = gmt::Vector<float>::Distance(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2));

    if (gmt::float_equals(length, 0.0f, min_arrow_size)) { return; }  // Dont draw if the vector is null

    gmt::Vector<float> bottom = gmt::Vector<float>(x1, y1);
    gmt::Vector<float> head = gmt::Vector<float>(x2, y2);
    gmt::Vector<float> normal = gmt::Vector<float>::Normal((head - bottom).Normalize());

    float ratio = xhead / gmt::Vector<float>::Distance(bottom, head);

    gmt::Vector<float> head_basis = bottom + (head - bottom) * (1 - ratio);

    gmt::Vector<float> basis_A = head_basis + normal * (yhead / 2.0f);
    gmt::Vector<float> basis_B = head_basis - normal * (yhead / 2.0f);

    DrawTriangle(head.x, head.y, basis_A.x, basis_A.y, basis_B.x, basis_B.y, color, this->vertices_buffer);
    DrawLine(bottom.x, bottom.y, head_basis.x, head_basis.y, thickness, color);

    this->arrows++;
}

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

void Renderer::DrawRectangle(int x1, int y1, int x2, int y2, bool fixed, sf::Color color, bool outline) {
    if (!gmt::Bounds<float>::BoundsIntersectBounds(gmt::Bounds<float>(x1, y1, x2, y2), get_screen_bounds())) { return; }

    gmt::Vector<float> pA = gmt::Vector<float>(x1, y1);
    gmt::Vector<float> pB = gmt::Vector<float>(x1, y2);
    gmt::Vector<float> pC = gmt::Vector<float>(x2, y2);
    gmt::Vector<float> pD = gmt::Vector<float>(x2, y1);

    if (outline) {
        this->DrawLine(pA.x, pA.y, pB.x, pB.y, outline_thickness, color);
        this->DrawLine(pB.x, pB.y, pC.x, pC.y, outline_thickness, color);
        this->DrawLine(pC.x, pC.y, pD.x, pD.y, outline_thickness, color);
        this->DrawLine(pD.x, pD.y, pA.x, pA.y, outline_thickness, color);
    } else {
        this->DrawTriangle(pA.x, pA.y, pB.x, pB.y, pC.x, pC.y, color, this->vertices_buffer);
        this->DrawTriangle(pA.x, pA.y, pC.x, pC.y, pD.x, pD.y, color, this->vertices_buffer);
    }

    this->rectangles++;
}

void Renderer::DrawPolygon(gmt::VerticesI points, sf::Color color, bool outline) {
    if (!gmt::Bounds<float>::BoundsIntersectBounds(points.Bounds(), get_screen_bounds())) { return; }

    if (outline) {
        for (int i = 1; i < points.vertices.size(); i++) {
            std::shared_ptr<gmt::VectorI> last = points.vertices[i - 1];
            std::shared_ptr<gmt::VectorI> curr = points.vertices[i];
            DrawLine(last->x, last->y, curr->x, curr->y, outline_thickness, color);
        }

        std::shared_ptr<gmt::VectorI> first = points.vertices[0];
        std::shared_ptr<gmt::VectorI> last = points.vertices[points.vertices.size() - 1];
        DrawLine(first->x, first->y, last->x, last->y, outline_thickness, color);

    } else {
        std::shared_ptr<gmt::VectorI> first = points.vertices[0];

        for (int i = 2; i < points.vertices.size(); i++) {
            std::shared_ptr<gmt::VectorI> last = points.vertices[i - 1];
            std::shared_ptr<gmt::VectorI> curr = points.vertices[i];
            DrawTriangle(first->x, first->y, last->x, last->y, curr->x, curr->y, color, this->vertices_buffer);
        }
    }

    this->polygons++;
}
*/

}  // namespace drw