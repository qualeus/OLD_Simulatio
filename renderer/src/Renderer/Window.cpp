#include "../../include/Renderer/Window.hpp"

namespace drw {

Window::Window(int width, int height, std::string title) : overlay(ovl::Overlay()) {
    this->width = width;
    this->height = height;
    this->title = title;
}

void Window::InitializeGLFW() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) throw std::runtime_error("glfwInit failed.");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    /* Create a windowed mode window and its OpenGL context */
    this->window = glfwCreateWindow(this->get_width(), this->get_height(), this->title.c_str(), nullptr, nullptr);

    if (!this->window) {
        this->Cleanup();
        throw std::runtime_error("glfwCreateWindow failed.");
    }

    // Setting some window callbacks
    glfwSetWindowSizeCallback(window, glfw_window_size_callback);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetCharCallback(window, glfw_char_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);

    glfwSetWindowUserPointer(window, this);
}

void Window::InitializeBGFX() {
    // Tell bgfx about our window
    bgfx::PlatformData platform_data = {};

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    platform_data.ndt = glfwGetX11Display();
    platform_data.nwh = (void *)(uintptr_t)glfwGetX11Window(this->window);
#elif BX_PLATFORM_OSX
    platform_data.nwh = glfwGetCocoaWindow(this->window);
#elif BX_PLATFORM_WINDOWS
    platform_data.nwh = glfwGetWin32Window(this->window);
#endif
    bgfx::setPlatformData(platform_data);

    // Init bgfx
    bgfx::Init init;
    // init.type = bgfx::RendererType::Vulkan;  // Select rendering backend
    init.vendorId = BGFX_PCI_ID_NONE;  // Choose graphics card vendor
    init.deviceId = 0;                 // Choose which graphics card to use
    init.callback = nullptr;

    if (!bgfx::init(init)) throw std::runtime_error("unable to initialize bgfx");
}

void Window::InitializeIMGUI() { this->overlay.Initialize(this->window); }

void Window::Render() {
    this->InitializeGLFW();
    this->InitializeBGFX();
    this->InitializeIMGUI();

    this->Reset();
}

void Window::Reset() {
    bgfx::reset(this->get_width(), this->get_height());

    this->overlay.Reset(this->get_width(), this->get_height());

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Window::Clear() {}

void Window::PreDraw() {
    // Calculate delta time
    this->current_time = (float)glfwGetTime();  // std::static_cast<float>(glfwGetTime());
    this->delta_time = this->current_time - this->last_time;
    this->last_time = this->current_time;

    // Poll events
    glfwPollEvents();
    this->overlay.PollEvents(this->delta_time);

    // Render
    ImGui::NewFrame();

    bgfx::touch(0);
}

void Window::Draw() {
    this->overlay.Draw();
    bgfx::frame();
}

void Window::Close() {
    glfwDestroyWindow(this->window);
    this->Cleanup();
}

void Window::Cleanup() {
    this->overlay.Cleanup();
    bgfx::shutdown();
    glfwTerminate();
}

bool Window::isOpen() { return !glfwWindowShouldClose(this->window); }
int Window::get_width() { return this->width; }
int Window::get_height() { return this->height; }

void Window::glfw_error_callback(int error, const char *description) { fprintf(stderr, "GLFW error %d: %s\n", error, description); }

void Window::glfw_window_size_callback(GLFWwindow *window, int width, int height) {
    Window *ctx = static_cast<Window *>(glfwGetWindowUserPointer(window));
    ctx->width = width;
    ctx->height = height;
    ctx->Reset();
}

void Window::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Window *ctx = static_cast<Window *>(glfwGetWindowUserPointer(window));
    ctx->overlay.glfw_key_callback(window, key, scancode, action, mods);
}

void Window::glfw_char_callback(GLFWwindow *window, unsigned int codepoint) {
    Window *ctx = static_cast<Window *>(glfwGetWindowUserPointer(window));
    ctx->overlay.glfw_char_callback(window, codepoint);
}

void Window::glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    Window *ctx = static_cast<Window *>(glfwGetWindowUserPointer(window));
    ctx->overlay.glfw_mouse_button_callback(window, button, action, mods);
}

void Window::glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Window *ctx = static_cast<Window *>(glfwGetWindowUserPointer(window));
    ctx->overlay.glfw_scroll_callback(window, xoffset, yoffset);
}

}  // namespace drw