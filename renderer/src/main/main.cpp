
#include "../../include/Drawing/Window.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

drw::Window window;
bool show_demo_window = true;

void loop() {
    window.Clear();

    /*
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.0, 0.5);
    glEnd();
    */

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    /* Swap front and back buffers */
    window.Draw();
}

int main(void) {
    window = drw::Window();
    window.Render();
    show_demo_window = true;

#ifdef __EMSCRIPTEN__
    // EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });
    //  EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });
    //  EM_JS(void, resizeCanvas, (), { js_resizeCanvas(); });
#endif

    /* Loop until the user closes the window */
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(&loop, 0, 1);
#else
    while (window.isOpen()) { loop(); }
#endif

    window.Close();
    return 0;
}