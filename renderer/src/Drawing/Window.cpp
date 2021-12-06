#include "../../include/Drawing/Window.hpp"

namespace drw {

Window::Window(int width, int height, std::string title) : camera(drw::Camera()) {
    this->size = glm::ivec2(width, height);
    this->title = title;
}

void Window::InitializeGLFW() {
    if (!glfwInit()) throw std::runtime_error("glfwInit failed.");

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
    this->window = glfwCreateWindow(this->get_width(), this->get_height(), this->title.c_str(), NULL, NULL);

    if (!this->window) {
        this->Cleanup();
        throw std::runtime_error("glfwCreateWindow failed.");
    }
}

void Window::InitializeBGFX() {
    bgfx::renderFrame();
    bgfx::Init init;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif

    glfwGetWindowSize(this->window, &this->size.x, &this->size.y);
    init.resolution.width = (uint32_t)this->get_width();
    init.resolution.height = (uint32_t)this->get_height();
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init)) throw std::runtime_error("bgfx::init failed.");

    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    bgfx::setViewClear(this->viewId, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(this->viewId, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Window::Render() {
    this->InitializeGLFW();
    this->InitializeBGFX();

    // glfwMakeContextCurrent(this->window);
    glewInit();
    // glfwSwapInterval(1);  // Enable vsync

    ovl::Overlay overlay = ovl::Overlay(this->window, this->glsl_version);
    this->overlay = &overlay;
    this->overlay->Initialize();
}

void Window::Clear() {
    // glClear(GL_COLOR_BUFFER_BIT);
    this->Prepare();
    bgfx::touch(this->viewId);
}

void Window::Draw() {
    this->overlay->Render();
    glfwGetFramebufferSize(this->window, &this->size.x, &this->size.y);
    glViewport(0, 0, this->get_width(), this->get_height());
    this->overlay->RenderDrawData();
    glfwSwapBuffers(this->window);
}

void Window::Prepare() {
    this->HandleResize();
    glfwPollEvents();
    this->overlay->Prepare();
}

void Window::HandleResize() {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    if (width != this->get_width() || height != this->get_height()) {
        this->size = glm::ivec2(width, height);
        bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
        bgfx::setViewRect(this->viewId, 0, 0, bgfx::BackbufferRatio::Equal);
    }
}

void Window::Cleanup() {
    bgfx::shutdown();
    glfwTerminate();
}
void Window::Close() {
    this->overlay->Cleanup();
    glfwDestroyWindow(this->window);
    this->Cleanup();
}

bool Window::isOpen() { return !glfwWindowShouldClose(this->window); }

int Window::get_width() { return this->size.x; }
int Window::get_height() { return this->size.y; }
}  // namespace drw