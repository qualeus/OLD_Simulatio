#include "../../include/Context/Renderer.hpp"

namespace ctx {

Renderer::Renderer() { this->window = Window(1000, 800, "Test"); }

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
    this->window.PreDraw();
    this->window.Clear();

    // Use debug font to print information about this example.
    bgfx::dbgTextClear();

    bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

    const bgfx::Stats* stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

    // Enable stats or debug text.
    bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    if (Inputs::KeyPressed(GLFW_KEY_F1)) {
        s_showStats = !s_showStats;
        std::cout << "F1 pressed" << std::endl;
    }
    if (Inputs::KeyPressed("A")) { std::cout << "A pressed" << std::endl; }
    if (Inputs::KeyDown("A")) { std::cout << "A down" << std::endl; }
    if (Inputs::KeyReleased("A")) { std::cout << "A released" << std::endl; }

    if (Inputs::KeyPressed(GLFW_KEY_ESCAPE)) { this->window.Close(); }

    this->window.Draw();
}

}  // namespace ctx