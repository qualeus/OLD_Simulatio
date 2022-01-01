#include "../../include/Drawing/Shapes.hpp"

namespace drw {

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

void Shapes::DrawTriangle(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3, uint32_t color) {
    const int vertices_size = mesh.vertices.size();

    mesh.vertices.push_back(Vertex<float>(pt1, color));
    mesh.vertices.push_back(Vertex<float>(pt2, color));
    mesh.vertices.push_back(Vertex<float>(pt3, color));

    mesh.indexes.push_back(vertices_size + 0);
    mesh.indexes.push_back(vertices_size + 1);
    mesh.indexes.push_back(vertices_size + 2);

    Shapes::triangles++;
}

uint32_t Shapes::triangles = 0;
uint32_t Shapes::rectangles = 0;
uint32_t Shapes::lines = 0;
uint32_t Shapes::arrows = 0;
uint32_t Shapes::circles = 0;
uint32_t Shapes::springs = 0;
uint32_t Shapes::polygons = 0;

/*
void Shapes::DrawQuad(int x, int y, int range, sf::Color color, std::vector<sf::Vertex> &buffer) {
    float x1 = static_cast<float>(x - range);
    float x2 = static_cast<float>(x + range);
    float y1 = static_cast<float>(y - range);
    float y2 = static_cast<float>(y + range);

    DrawTriangleTexture(x1, y1, x1, y2, x2, y1, 0, 0, 0, 1, 1, 0, color, buffer);
    DrawTriangleTexture(x2, y2, x1, y2, x2, y1, 1, 1, 0, 1, 1, 0, color, buffer);
}

void Shapes::DrawLine(int x1, int y1, int x2, int y2, int thickness, sf::Color color) {
    gmt::Vector<float> oA = gmt::Vector<float>(x1, y1);
    gmt::Vector<float> oB = gmt::Vector<float>(x2, y2);

    if (!gmt::Bounds<float>::SegmentIntersectBounds(oA, oB, get_screen_bounds())) { return; }

    gmt::Vector<float> normal = gmt::Vector<float>::Normal(oB - oA).Normalize() * (thickness / 2.0f);

    gmt::Vector<float> pA = oA - normal;
    gmt::Vector<float> pB = oA + normal;
    gmt::Vector<float> pC = oB - normal;
    gmt::Vector<float> pD = oB + normal;

    this->DrawTriangle(pA.x, pA.y, pB.x, pB.y, pC.x, pC.y, color, this->vertices_buffer);
    this->DrawTriangle(pB.x, pB.y, pC.x, pC.y, pD.x, pD.y, color, this->vertices_buffer);

    this->lines++;
}

void Shapes::DrawSpring(int x1, int y1, int x2, int y2, int thickness, int number_wave, sf::Color color) {
    if (!gmt::Bounds<float>::SegmentIntersectBounds(gmt::Vector<float>(x1, y1), gmt::Vector<float>(x2, y2), get_screen_bounds())) { return; }

    float inv = 0.25f / number_wave;
    float dx = (x2 - x1) * inv;
    float dy = (y2 - y1) * inv;

    float inv2 = thickness / std::sqrt(dx * dx + dy * dy);
    float px = dy * inv2;
    float py = -dx * inv2;

    float x = x1;
    float y = y1;
    for (int i = 0; i < number_wave; i++) {
        DrawLine(x, y, x + dx + px, y + dy + py, 2.0f, color);
        DrawLine(x + dx + px, y + dy + py, x + 3.0f * dx - px, y + 3.0f * dy - py, 2.0f, color);
        DrawLine(x + 3.0f * dx - px, y + 3.0f * dy - py, x + 4.0f * dx, y + 4.0f * dy, 2.0f, color);
        x += 4.0f * dx;
        y += 4.0f * dy;
    }

    this->springs++;
}

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
}*/

}  // namespace drw