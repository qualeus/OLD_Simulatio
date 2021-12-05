
#include "../../include/Drawing/Window.hpp"

int main(void) {
    drw::Window window = drw::Window();
    window.Render();

    bool show_demo_window = true;

    /* Loop until the user closes the window */
    while (window.isOpen()) {
        window.Clear();
        window.Prepare();

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.0, 0.5);
        glEnd();

        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        /* Swap front and back buffers */
        window.Draw();
    }

    window.Close();
    return 0;
}