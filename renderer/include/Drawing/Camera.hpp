#ifndef Camera_HPP
#define Camera_HPP
#include <GLFW/glfw3.h>

#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace drw {

class Camera {
   private:
    glm::vec3 position;
    glm::vec3 towards;
    glm::vec2 size;

   public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 towards = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2 size = glm::vec2(0.0f, 0.0f));

    void set_size(glm::vec2 size);
    glm::vec2 get_size() const;

    void set_position(glm::vec3 position);
    glm::vec3 get_position() const;

    void set_towards(glm::vec3 towards);
    glm::vec3 get_towards() const;

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_proj_matrix() const;
};

}  // namespace drw
#endif