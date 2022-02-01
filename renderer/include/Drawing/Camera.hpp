#ifndef Camera_HPP
#define Camera_HPP
#include <GLFW/glfw3.h>

#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace drw {

class Camera {
   private:
    glm::vec3 position;
    glm::vec3 towards;
    glm::vec4 viewport;

    glm::mat4 view;
    glm::mat4 proj;

   public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 towards = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    void set_position(glm::vec3 position);
    glm::vec3 get_position() const;

    void set_towards(glm::vec3 towards);
    glm::vec3 get_towards() const;

    void set_view_matrix(float* view_matrix);
    glm::mat4 get_view_matrix() const;

    void set_proj_matrix(float* proj_matrix);
    glm::mat4 get_proj_matrix() const;

    void set_viewport(glm::vec4 point);
    glm::vec4 get_viewport() const;

    glm::vec3 projectPoint(glm::vec3 point) const;
};

}  // namespace drw
#endif