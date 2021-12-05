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

void Camera::ResetKeyboardInputs() {
    memset(this->pressed_keys, false, sizeof(this->pressed_keys));
    memset(this->released_keys, false, sizeof(this->released_keys));
}

void Camera::ResetMouseInputs() {
    memset(this->pressed_mouse, false, sizeof(this->pressed_mouse));
    memset(this->released_mouse, false, sizeof(this->released_mouse));
}

void Camera::HandleKeyboardInputs(int key, int action) {
    if (key >= 0 && key < 512) {
        if (action == GLFW_PRESS) {
            this->current_keys[key] = true;
            this->pressed_keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            this->current_keys[key] = false;
            this->released_keys[key] = true;
        }
    }
}

void Camera::HandleMouseInputs(int button, int action) {
    if (button >= 0 && button < 3) {
        if (action == GLFW_PRESS) {
            this->current_mouse[button] = true;
            this->pressed_mouse[button] = true;
        } else if (action == GLFW_RELEASE) {
            this->current_mouse[button] = false;
            this->released_mouse[button] = true;
        }
    }
}

}  // namespace drw