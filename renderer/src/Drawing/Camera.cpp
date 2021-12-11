#include "../../include/Drawing/Camera.hpp"

namespace drw {
Camera::Camera(glm::vec3 position, glm::vec3 towards, glm::vec2 size) {
    this->position = position;
    this->towards = towards;
    this->size = size;
}

void Camera::set_size(glm::vec2 size) {
    // w,h become zero when glfw window is minimized
    this->size = glm::vec2(size.x == 0 ? 1 : size.x, size.y == 0 ? 1 : size.y);
}

glm::mat4 Camera::get_view_matrix() const {
    // position, towards, updwards
    return glm::lookAt(this->position, this->towards, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::get_proj_matrix() const {
    // perspective
    return glm::perspectiveFov(45.0f, this->size.x, this->size.y, 0.1f, 10000.0f);
}

}  // namespace drw