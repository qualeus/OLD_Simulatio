#include "../../include/Drawing/Camera.hpp"

namespace drw {
Camera::Camera(glm::vec3 position, glm::vec3 towards, glm::vec2 size) {
    this->position = position;
    this->towards = towards;
    this->size = size;
}

glm::vec3 Camera::get_position() const { return this->position; }
void Camera::set_position(glm::vec3 position) { this->position = position; }

glm::vec3 Camera::get_towards() const { return this->towards; }
void Camera::set_towards(glm::vec3 towards) { this->towards = towards; }

glm::vec2 Camera::get_size() const { return this->size; }
void Camera::set_size(glm::vec2 size) {
    this->size = glm::vec2(size.x == 0 ? 1 : size.x, size.y == 0 ? 1 : size.y);  // w,h become zero when glfw window is minimized
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(this->position, this->towards, glm::vec3(0.0f, 1.0f, 0.0f));  // position, towards, updwards
}

glm::mat4 Camera::get_proj_matrix() const {
    return glm::perspectiveFov(45.0f, this->size.x, this->size.y, 0.1f, 10000.0f);  // perspective
}

}  // namespace drw