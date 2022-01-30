#include "../../include/Context/Renderer.hpp"

namespace ctx {

Renderer::Renderer() : system(phy::System()), overlay(ovl::GuiManager(&this->system)) {
    this->window = Window(1000, 800, "Test");
    this->camera = drw::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f));

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
    this->system.Step();

    this->window.PreDraw();
    drw::Shapes::Reset();

    this->Debug();

    this->overlay.DrawGui();

    this->UpdateCamera();

    this->meshes["base"] = drw::Mesh();
    // this->meshes["circle"] = drw::Mesh();

    this->DrawSystem();

    drw::Shapes::Draw(this->meshes["base"], this->shaders["base"]);
    // drw::Shapes::Draw(this->meshes["circle"], this->shaders["circle"]);

    this->Inputs();

    this->window.Draw();
}

void Renderer::UpdateCamera() {
    const glm::vec3 cpos = this->camera.get_position();
    const glm::vec3 ctrd = this->camera.get_towards();

    const bx::Vec3 at = {cpos.x, cpos.y, cpos.z};
    const bx::Vec3 eye = {ctrd.x, ctrd.y, ctrd.z};

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
}

void Renderer::Debug() {
    bgfx::dbgTextClear();
    const bgfx::Stats *stats = bgfx::getStats();

    const glm::vec3 cam_pos = this->camera.get_position();
    const glm::vec2 cur_pos = Inputs::MousePosition();

    bgfx::dbgTextPrintf(0, 0, 0x0f, "\x1b[15;8m SIMULATIO v%i.%i.%i [%s ] \x1b[0m",              //
                        PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_REVISION,  //
                        std::string(gmt::type_name<SIMULATIO_PRECISION>()).c_str());             //
    bgfx::dbgTextPrintf(0, 1, 0x0f, "\x1b[15;8m Camera (%f; %f; %f)\x1b[0m", cam_pos.x, cam_pos.y, cam_pos.z);
    bgfx::dbgTextPrintf(0, 2, 0x0f, "\x1b[15;8m Cursor (%f; %f)\x1b[0m", cur_pos.x, cur_pos.y);

    switch (debug) {
        case 0: bgfx::dbgTextClear(); break;
        case 1: bgfx::setDebug(BGFX_DEBUG_STATS); break;
        case 2: bgfx::setDebug(BGFX_DEBUG_TEXT); break;
        default: break;
    }
}

void Renderer::Inputs() {
    Inputs::HandleMousePosition();

    GlobalInputs();
    DebugInputs();
    CameraInputs();
}

void Renderer::GlobalInputs() {
    if (Inputs::KeyPressed(GLFW_KEY_ESCAPE)) { this->window.Close(); }
}

void Renderer::DebugInputs() {
    if (Inputs::KeyPressed(GLFW_KEY_F1)) { debug = (debug + 1) % 3; }
    if (Inputs::KeyPressed(GLFW_KEY_A)) {
        const glm::vec2 mouse_pos = Inputs::MousePosition();
        addCorpse(phy::Circle(mouse_pos.x / (float)this->window.get_width(), mouse_pos.y / (float)this->window.get_height(), 1.0), 0xffffffff);
    }
}

void Renderer::CameraInputs() {
    const float CAMERA_STEP = 0.005f;
    const float CAMERA_ZOOM = 0.01f;

    if (Inputs::KeyDown(GLFW_KEY_LEFT)) {
        this->camera.set_position(this->camera.get_position() + glm::vec3(CAMERA_STEP, 0, 0));
        this->camera.set_towards(this->camera.get_towards() + glm::vec3(CAMERA_STEP, 0, 0));
    }

    if (Inputs::KeyDown(GLFW_KEY_RIGHT)) {
        this->camera.set_position(this->camera.get_position() - glm::vec3(CAMERA_STEP, 0, 0));
        this->camera.set_towards(this->camera.get_towards() - glm::vec3(CAMERA_STEP, 0, 0));
    }

    if (Inputs::KeyDown(GLFW_KEY_UP)) {
        this->camera.set_position(this->camera.get_position() + glm::vec3(0, CAMERA_STEP, 0));
        this->camera.set_towards(this->camera.get_towards() + glm::vec3(0, CAMERA_STEP, 0));
    }

    if (Inputs::KeyDown(GLFW_KEY_DOWN)) {
        this->camera.set_position(this->camera.get_position() - glm::vec3(0, CAMERA_STEP, 0));
        this->camera.set_towards(this->camera.get_towards() - glm::vec3(0, CAMERA_STEP, 0));
    }

    if (Inputs::KeyDown(GLFW_KEY_P)) {
        this->camera.set_position(this->camera.get_position() + glm::vec3(0, 0, CAMERA_ZOOM));
        this->camera.set_towards(this->camera.get_towards() + glm::vec3(0, 0, CAMERA_ZOOM));
    }

    if (Inputs::KeyDown(GLFW_KEY_O)) {
        this->camera.set_position(this->camera.get_position() - glm::vec3(0, 0, CAMERA_ZOOM));
        this->camera.set_towards(this->camera.get_towards() - glm::vec3(0, 0, CAMERA_ZOOM));
    }

    if (Inputs::MousePressed(GLFW_MOUSE_BUTTON_1)) {
        this->mouse_initial_position = this->camera.get_position();
        this->mouse_initial_towards = this->camera.get_towards();
    }

    if (Inputs::MouseDown(GLFW_MOUSE_BUTTON_1)) {
        const float scroll_ratio = CAMERA_STEP * 0.29f * this->camera.get_towards().z;
        const glm::vec2 mouse_diff = Inputs::PressedMouseDiff(GLFW_MOUSE_BUTTON_1);

        // (float)this->window.get_width() / (float)this->window.get_height();

        const glm::vec3 position_diff = glm::vec3(mouse_diff.x * scroll_ratio, mouse_diff.y * scroll_ratio, 0);

        this->camera.set_position(this->mouse_initial_position + position_diff);
        this->camera.set_towards(this->mouse_initial_towards + position_diff);
    }

    const double SCROLL_TOGGLE_OFFSET = 0.1;
    if (Inputs::MouseScroll() < -SCROLL_TOGGLE_OFFSET || Inputs::MouseScroll() > SCROLL_TOGGLE_OFFSET) {
        this->camera.set_position(this->camera.get_position() - glm::vec3(0, 0, Inputs::MouseScroll() * 100.0 * CAMERA_ZOOM));
        this->camera.set_towards(this->camera.get_towards() - glm::vec3(0, 0, Inputs::MouseScroll() * 100.0 * CAMERA_ZOOM));
    }
}

void Renderer::LoadShaders() {
    const bgfx::Memory *base_vs = drw::Shader::get_base_vs_shader();
    const bgfx::Memory *base_fs = drw::Shader::get_base_fs_shader();

    this->shaders["base"] = drw::Shader::create_program("base", base_vs, base_fs);

    const bgfx::Memory *circle_vs = drw::Shader::get_circle_vs_shader();
    const bgfx::Memory *circle_fs = drw::Shader::get_circle_fs_shader();

    this->shaders["circle"] = drw::Shader::create_program("circle", circle_vs, circle_fs);
}

void Renderer::addCorpse(phy::Polygon polygon, uint32_t color) {
    this->corpses_colors[polygon.get_id()] = color;
    this->system.addCorpse(polygon);
}

void Renderer::addCorpse(phy::Circle circle, uint32_t color) {
    this->corpses_colors[circle.get_id()] = color;
    this->system.addCorpse(circle);
}

std::shared_ptr<phy::Corpse> Renderer::getCorpse(int index) const { return this->system.get_corpse(index); }

void Renderer::addConstraint(phy::Link link, uint32_t color) {
    this->constraints_colors[link.get_id()] = color;
    this->system.addConstraint(link);
}

void Renderer::addConstraint(phy::Spring spring, uint32_t color) {
    this->constraints_colors[spring.get_id()] = color;
    this->system.addConstraint(spring);
}

void Renderer::addConstraint(phy::Slider slider, uint32_t color) {
    this->constraints_colors[slider.get_id()] = color;
    this->system.addConstraint(slider);
}

std::shared_ptr<phy::Constraint> Renderer::getConstraint(int index) const { return this->system.get_constraint(index); }

void Renderer::DrawSystem() {
    for (int i = 0; i < this->system.get_corpses_size(); i++) { DrawCorpse(this->system.get_corpse(i), this->corpses_colors[system.get_corpse(i)->get_id()]); }
    for (int i = 0; i < this->system.get_constraints_size(); i++) { DrawConstraint(this->system.get_constraint(i), this->constraints_colors[system.get_constraint(i)->get_id()]); }

    // drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 0xffffffff);

    // drw::Shapes::DrawQuad(this->meshes["base"], glm::vec3(-1, 0, 0), 1, 0xffffffff);
    // drw::Shapes::DrawLine(this->meshes["base"], glm::vec3(-10, 0, 0), glm::vec3(10, 2, 0), 0.1f, 0xffffffff);

    // drw::Shapes::DrawSpring(this->meshes["base"], glm::vec3(-10, 0, 0), glm::vec3(10, 2, 0), 0.1f, 1.0f, 10, 0xffffffff);
    // drw::Shapes::DrawArrow(this->meshes["base"], glm::vec3(10, 0, 0), glm::vec3(0, 0, 0), 0.1f, 1.0f, 0xffffffff);

    // drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(10, 0, 5), glm::vec3(2, -7, 3), glm::vec3(1, 2, 0), 0xff000000);
    // drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 3, 3), 0xff000000);
    // for (int i = 0; i < 10000; i++) { drw::Shapes::DrawTriangle(this->meshes["base"], glm::vec3(-10, 0, 0), glm::vec3(1, 1, 0), glm::vec3(10, 3, 3), 0xff000000); }
}

void Renderer::DrawCorpse(std::shared_ptr<phy::Corpse> corpse, uint32_t color) {
    if (phy::Circle *circle = dynamic_cast<phy::Circle *>(corpse.get())) {
        DrawCorpseCircle(circle, color);

    } else if (phy::Polygon *polygon = dynamic_cast<phy::Polygon *>(corpse.get())) {
        DrawCorpsePolygon(polygon, color);
    }
}

void Renderer::DrawConstraint(std::shared_ptr<phy::Constraint> constraint, uint32_t color) {
    if (phy::Link *link = dynamic_cast<phy::Link *>(constraint.get())) {
        DrawConstraintLink(link, color);
    } else if (phy::Spring *spring = dynamic_cast<phy::Spring *>(constraint.get())) {
        DrawConstraintSpring(spring, color);
    } else if (phy::Slider *slider = dynamic_cast<phy::Slider *>(constraint.get())) {
        DrawConstraintSlider(slider, color);
    }
}

void Renderer::DrawCorpseCircle(phy::Circle *circle, uint32_t color) {
    const glm::vec3 center = glm::vec3(circle->get_pos_x(), circle->get_pos_y(), 0);
    drw::Shapes::DrawQuad(this->meshes["base"], center, circle->get_size(), color);
}

void Renderer::DrawCorpsePolygon(phy::Polygon *polygon, uint32_t color) {
    std::vector<gmt::VerticesI> polygon_triangles = polygon->get_polygons();

    for (int i = 0; i < polygon_triangles.size(); i++) {
        gmt::VerticesI triangle_vertices = polygon_triangles[i];
        std::vector<glm::vec3> polygon_points = {};

        for (int j = 0; j < triangle_vertices.vertices.size(); j++) {
            const std::shared_ptr<gmt::VectorI> vertice = triangle_vertices.vertices[j];
            polygon_points.push_back(glm::vec3(vertice->x, vertice->y, 0));
        }

        drw::Shapes::DrawPolygon(this->meshes["base"], polygon_points, color);
    }
}

void Renderer::DrawConstraintLink(phy::Link *link, uint32_t color) {}
void Renderer::DrawConstraintSpring(phy::Spring *spring, uint32_t color) {}
void Renderer::DrawConstraintSlider(phy::Slider *slider, uint32_t color) {}

}  // namespace ctx