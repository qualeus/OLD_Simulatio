#include "../../include/Drawing/Window.hpp"

namespace drw {

Window::Window(int width, int height, std::string title) {
    this->width = width;
    this->height = height;
    this->title = title;
}

void Window::Render() {
    /* Initialize the library */
    if (!glfwInit()) return;

#if defined(__APPLE__)  // GL 3.2 + GLSL 150
    this->glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    this->glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    /* Create a windowed mode window and its OpenGL context */
    this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);

    if (!this->window) {
        this->Cleanup();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);  // Enable vsync

    ovl::Overlay overlay = ovl::Overlay(this->window, this->glsl_version);
    this->overlay = &overlay;
    this->overlay->Initialize();
}

void Window::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
    this->Prepare();
}

void Window::Draw() {
    this->overlay->Render();
    glfwGetFramebufferSize(window, &this->width, &this->height);
    glViewport(0, 0, this->width, this->height);
    this->overlay->RenderDrawData();
    glfwSwapBuffers(this->window);
}

void Window::Prepare() {
    glfwPollEvents();
    this->overlay->Prepare();
}

void Window::Cleanup() { glfwTerminate(); }
void Window::Close() {
    this->overlay->Cleanup();
    glfwDestroyWindow(this->window);
    this->Cleanup();
}

bool Window::isOpen() { return !glfwWindowShouldClose(this->window); }

}  // namespace drw