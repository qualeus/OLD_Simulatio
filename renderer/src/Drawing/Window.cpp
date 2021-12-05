#include "../../include/Drawing/Window.hpp"

namespace drw {

Window::Window() {}
void Window::Render() {
        /* Initialize the library */
    if (!glfwInit()) return;

    /* Create a windowed mode window and its OpenGL context */
    this->window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!this->window) {
        glfwTerminate();
        return;
    }

    std::cout << this->window;

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(this->window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.0, 0.5);

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(this->window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}
void Window::Close() {}

}  // namespace drw