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

void Camera::set_view_matrix(float* view_matrix) {
    glm::mat4 tview_matrix = glm::mat4();
    for (int i = 0; i < 16; i++) { view_matrix[i] = view_matrix[i]; }
    this->view = tview_matrix;
}
glm::mat4 Camera::get_view_matrix() const { return this->view; }

void Camera::set_proj_matrix(float* proj_matrix) {
    glm::mat4 tproj_matrix = glm::mat4();
    for (int i = 0; i < 16; i++) { proj_matrix[i] = proj_matrix[i]; }
    this->proj = tproj_matrix;
}
glm::mat4 Camera::get_proj_matrix() const {
    return this->proj;  // perspective
}

}  // namespace drw