#include "../../include/Context/Renderer.hpp"

namespace ctx {

Renderer::Renderer() : system(phy::System()), overlay(ovl::GuiManager(&this->system)) {
    this->window = Window(1000, 800, "Test");

    this->shaders = {};
    this->meshes = {};
}

void Renderer::Render() {
    this->window.Render();
    this->overlay.Setup();

#ifdef __EMSCRIPTEN__
    // EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });
    // EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });
    // EM_JS(void, resizeCanvas, (), { js_resizeCanvas(); });
#endif

    /* Loop until the user closes the window */
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(&this->Loop, 0, 1);
#else

    this->LoadShaders();

    while (this->window.isOpen()) { this->Loop(); }
#endif

    this->window.Close();
}

void Renderer::Loop() {
    this->window.PreDraw();
    drw::Shapes::Reset();

    this->Debug();

    // this->overlay.DrawGui();

    // ==================================================================

    const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, 10.0f};

    // Set view and projection matrix for view 0.
    float view[16];
    bx::mtxLookAt(view, eye, at);

    float proj[16];
    bx::mtxProj(proj, 60.0f, (float)this->window.get_width() / (float)this->window.get_height(), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

    float mtx[16];
    bx::mtxRotateY(mtx, 0.0f);

    // position x,y,z
    mtx[12] = 0.0f;
    mtx[13] = 0.0f;
    mtx[14] = 0.0f;

    // Set model matrix for rendering.
    bgfx::setTransform(mtx);

    this->meshes["base"] = drw::Mesh();
    drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 0xff000000);
    drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(10, 0, 5), glm::vec3(2, -7, 3), glm::vec3(1, 2, 0), 0xff000000);
    drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 3, 3), 0xff000000);
    for (int i = 0; i < 10000; i++) { drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(-10, 0, 0), glm::vec3(1, 1, 0), glm::vec3(10, 3, 3), 0xff000000); }

    drw::Shapes::Draw(this->meshes["base"], this->shaders["base"]);

    // ==================================================================

    this->Inputs();

    this->window.Draw();
}

void Renderer::Debug() {
    bgfx::dbgTextClear();
    const bgfx::Stats* stats = bgfx::getStats();

    bgfx::dbgTextPrintf(0, 1, 0x0f, "Press F1 to toggle stats.");

    bgfx::dbgTextPrintf(0, 2, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    bgfx::dbgTextPrintf(0, 3, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 3, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

    switch (debug) {
        case 0: bgfx::dbgTextClear(); break;
        case 1: bgfx::setDebug(BGFX_DEBUG_STATS); break;
        case 2: bgfx::setDebug(BGFX_DEBUG_TEXT); break;
        default: break;
    }
}

void Renderer::Inputs() {
    if (Inputs::KeyPressed(GLFW_KEY_F1)) { debug = (debug + 1) % 3; }
    if (Inputs::KeyPressed(GLFW_KEY_ESCAPE)) { this->window.Close(); }
    if (Inputs::KeyPressed(GLFW_KEY_A)) { std::cout << gmt::to_string(this->system.get_corpses()) << std::endl; }
}

void Renderer::LoadShaders() {
    const bgfx::Memory* base_vs = drw::Shader::get_base_vs_shader();
    const bgfx::Memory* base_fs = drw::Shader::get_base_fs_shader();

    this->shaders["base"] = drw::Shader::create_program("base", base_vs, base_fs);
}

}  // namespace ctx