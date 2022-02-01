#include "../../include/Context/Renderer.hpp"

namespace ctx {

Renderer::Renderer() : system(phy::System()), overlay(ovl::GuiManager(&this->system)) {
    const std::string title = "Test";
    const std::string sim_title = "Simulatio v" + std::to_string(PROJECT_VERSION_MAJOR) + "."  //
                                  + std::to_string(PROJECT_VERSION_MINOR) + "."                //
                                  + std::to_string(PROJECT_VERSION_REVISION)                   //
                                  + " // " + title;                                            //

    this->window = Window(1000, 800, sim_title);
    this->camera = drw::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f));
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

    this->DeclareMeshes();

    this->DrawSystem();

    this->DrawMeshes();

    this->Inputs();

    this->window.Draw();
}

void Renderer::DeclareMeshes() {
    this->base_mesh = this->DeclareColorMesh();
    this->circle_mesh = this->DeclareTextureMesh();
}

void Renderer::DrawMeshes() {
    // Base Mesh => Colors vertex
    drw::Shapes::Draw(this->base_mesh, this->base_shader);

    // Circle Mesh => Texture vertex
    drw::Shapes::Draw(this->circle_mesh, this->circle_shader);
}

drw::Mesh<drw::VertexCol> Renderer::DeclareColorMesh() {
    drw::Mesh<drw::VertexCol> mesh = drw::Mesh<drw::VertexCol>();

    mesh.declaration.begin();                                                      // init
    mesh.declaration.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);      // vertex
    mesh.declaration.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);  // color
    mesh.declaration.end();                                                        // stop

    return mesh;
}

drw::Mesh<drw::VertexTex> Renderer::DeclareTextureMesh() {
    drw::Mesh<drw::VertexTex> mesh = drw::Mesh<drw::VertexTex>();

    mesh.declaration.begin();                                                      // init
    mesh.declaration.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);      // vertex
    mesh.declaration.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);     // texture
    mesh.declaration.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);  // color
    mesh.declaration.end();                                                        // stop

    return mesh;
}

void Renderer::UpdateCamera() {
    const glm::vec3 cpos = this->camera.get_position();
    const glm::vec3 ctrd = this->camera.get_towards();

    const bx::Vec3 at = {cpos.x, cpos.y, cpos.z};
    const bx::Vec3 eye = {ctrd.x, ctrd.y, ctrd.z};

    // Set view and projection matrix for view 0.
    float view[16];
    bx::mtxLookAt(view, eye, at);
    this->camera.set_view_matrix(view);

    float proj[16];
    bx::mtxProj(proj, 60.0f, (float)this->window.get_width() / (float)this->window.get_height(), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    this->camera.set_proj_matrix(proj);

    bgfx::setViewTransform(0, view, proj);
    this->camera.set_viewport(glm::vec4(0, 0, (float)this->window.get_width(), (float)this->window.get_height()));

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
    const glm::vec3 cam_tw = this->camera.get_towards();
    const glm::vec4 cam_vp = this->camera.get_viewport();
    const glm::mat4 cam_vw = this->camera.get_view_matrix();
    const glm::mat4 cam_pr = this->camera.get_proj_matrix();
    const glm::vec2 cur_pos = Inputs::MousePosition();
    const glm::vec3 cur_prj = this->camera.projectPoint(glm::vec3(cur_pos.x, cur_pos.y, -10));

    bgfx::dbgTextPrintf(0, 0, 0x0f, "\x1b[15;8m SIMULATIO v%i.%i.%i [%s ] \x1b[0m",              //
                        PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_REVISION,  //
                        std::string(gmt::type_name<SIMULATIO_PRECISION>()).c_str());             //
    bgfx::dbgTextPrintf(0, 1, 0x0f, "\x1b[15;8m Camera: position (%f; %f; %f)\x1b[0m", cam_pos.x, cam_pos.y, cam_pos.z);
    bgfx::dbgTextPrintf(0, 2, 0x0f, "\x1b[15;8m         towards (%f; %f; %f)\x1b[0m", cam_tw.x, cam_tw.y, cam_tw.z);
    bgfx::dbgTextPrintf(0, 3, 0x0f, "\x1b[15;8m         viewport (%f; %f; %f; %f)\x1b[0m", cam_vp[0], cam_vp[1], cam_vp[2], cam_vp[3]);

    bgfx::dbgTextPrintf(0, 5, 0x0f, "\x1b[15;8m         view l1 [%f; %f; %f; %f]", cam_vw[0], cam_vw[1], cam_vw[2], cam_vw[3]);
    bgfx::dbgTextPrintf(0, 6, 0x0f, "\x1b[15;8m         view l2 [%f; %f; %f; %f]", cam_vw[4], cam_vw[5], cam_vw[6], cam_vw[7]);
    bgfx::dbgTextPrintf(0, 7, 0x0f, "\x1b[15;8m         view l3 [%f; %f; %f; %f]", cam_vw[8], cam_vw[9], cam_vw[10], cam_vw[11]);
    bgfx::dbgTextPrintf(0, 8, 0x0f, "\x1b[15;8m         view l4 [%f; %f; %f; %f]", cam_vw[12], cam_vw[13], cam_vw[14], cam_vw[15]);

    bgfx::dbgTextPrintf(0, 10, 0x0f, "\x1b[15;8m         proj l1 [%f; %f; %f; %f]", cam_pr[0], cam_pr[1], cam_pr[2], cam_pr[3]);
    bgfx::dbgTextPrintf(0, 11, 0x0f, "\x1b[15;8m         proj l2 [%f; %f; %f; %f]", cam_pr[4], cam_pr[5], cam_pr[6], cam_pr[7]);
    bgfx::dbgTextPrintf(0, 12, 0x0f, "\x1b[15;8m         proj l3 [%f; %f; %f; %f]", cam_pr[8], cam_pr[9], cam_pr[10], cam_pr[11]);
    bgfx::dbgTextPrintf(0, 13, 0x0f, "\x1b[15;8m         proj l4 [%f; %f; %f; %f]", cam_pr[12], cam_pr[13], cam_pr[14], cam_pr[15]);

    bgfx::dbgTextPrintf(0, 15, 0x0f, "\x1b[15;8m Cursor: position (%f; %f)\x1b[0m", cur_pos.x, cur_pos.y);
    bgfx::dbgTextPrintf(0, 16, 0x0f, "\x1b[15;8m         projection (%f; %f; %f)\x1b[0m", cur_prj.x, cur_prj.y, cur_prj.z);

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
    const float CAMERA_ZOOM = 0.05f;

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

    this->base_shader = drw::Shader::create_program("base", base_vs, base_fs);

    const bgfx::Memory *circle_vs = drw::Shader::get_circle_vs_shader();
    const bgfx::Memory *circle_fs = drw::Shader::get_circle_fs_shader();

    this->circle_shader = drw::Shader::create_program("circle", circle_vs, circle_fs);
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
    for (int i = 0; i < this->system.get_corpses_size(); i++) {
        DrawCorpse(this->system.get_corpse(i), this->corpses_colors[system.get_corpse(i)->get_id()]);  //
    }

    for (int i = 0; i < this->system.get_constraints_size(); i++) {
        DrawConstraint(this->system.get_constraint(i), this->constraints_colors[system.get_constraint(i)->get_id()]);  //
    }
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
    // drw::Shapes::DrawQuad(this->circle_mesh, center, circle->get_size(), color);
    // drw::Shapes::DrawQuad(this->base_mesh, center, circle->get_size(), color);
    drw::Shapes::DrawCircleFan(this->base_mesh, center, circle->get_size() / 2.0f, color);
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

        drw::Shapes::DrawPolygon(this->base_mesh, polygon_points, color);
    }
}

void Renderer::DrawConstraintLink(phy::Link *link, uint32_t color) {}
void Renderer::DrawConstraintSpring(phy::Spring *spring, uint32_t color) {}
void Renderer::DrawConstraintSlider(phy::Slider *slider, uint32_t color) {}

}  // namespace ctx