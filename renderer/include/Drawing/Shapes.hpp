#ifndef Shapes_HPP
#define Shapes_HPP

#include <GLFW/glfw3.h>

#include <Geometry/String.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Mesh.hpp"
#include "Vertex.hpp"

namespace drw {

#define BUFFER_SIZE 300000

class Shapes {
   private:
    Shapes();

   public:
    static uint32_t triangles;
    static uint32_t rectangles;
    static uint32_t lines;
    static uint32_t arrows;
    static uint32_t circles;
    static uint32_t springs;
    static uint32_t polygons;

    static void Draw(const Mesh& mesh, const bgfx::ProgramHandle& program);

    static void DrawTriangle(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3, uint32_t color);
    static void DrawPlane(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3, const glm::vec3& pt4, uint32_t color);
    static void DrawQuad(Mesh& mesh, const glm::vec3& center, const float& radius, uint32_t color);
    static void DrawLine(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, uint32_t color);
    static void DrawSpring(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, const float& width, const int& number_waves, uint32_t color);
    static void DrawArrow(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, const float& sz_head, uint32_t color);
    static void DrawRectangle(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, uint32_t color);
    static void DrawRectangleOutlined(Mesh& mesh, const glm::vec3& pt1, const glm::vec3& pt2, const float& thickness, uint32_t color);
    static void DrawPolygon(Mesh& mesh, const std::vector<glm::vec3>& points, uint32_t color);
    static void DrawPolygonOutlined(Mesh& mesh, const std::vector<glm::vec3>& points, const float& thickness, uint32_t color);

    static void Reset();
};

}  // namespace drw
#endif