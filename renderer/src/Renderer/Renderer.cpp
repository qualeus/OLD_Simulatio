#include "../../include/Renderer/Renderer.hpp"

namespace rdr {

Renderer::Renderer() { this->window = drw::Window(1000, 800, "Test"); }

void Renderer::Render() {
    this->window.Render();

#ifdef __EMSCRIPTEN__
    // EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });
    // EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });
    // EM_JS(void, resizeCanvas, (), { js_resizeCanvas(); });
#endif

    /* Loop until the user closes the window */
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(&this->Loop, 0, 1);
#else
    while (this->window.isOpen()) { this->Loop(); }
#endif

    this->window.Close();
}

void Renderer::Loop() {
    this->window.Clear();

    if (this->show_demo_window) ImGui::ShowDemoWindow(&this->show_demo_window);

    /* Swap front and back buffers */
    this->window.Draw();
}

}  // namespace rdr