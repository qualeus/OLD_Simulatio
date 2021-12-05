#ifndef Camera_HPP
#define Camera_HPP
#include <GL/glew.h>
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

    GLboolean pressed_mouse[3] = {false};
    GLboolean current_mouse[3] = {false};
    GLboolean released_mouse[3] = {false};

    GLboolean pressed_keys[512] = {false};
    GLboolean current_keys[512] = {false};
    GLboolean released_keys[512] = {false};

   public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 towards = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2 size = glm::vec2(0.0f, 0.0f));

    void set_size(glm::vec2 size);
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_proj_matrix() const;

    void ResetKeyboardInputs();
    void ResetMouseInputs();
    void HandleKeyboardInputs(int key, int action);
    void HandleMouseInputs(int button, int action);
};

}  // namespace drw
#endif