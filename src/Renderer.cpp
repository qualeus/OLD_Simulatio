#include "../include/Renderer.hpp"

Renderer::Renderer(float camera_x, float camera_y, float camera_h, float camera_w, float zoom, std::string p_name, bool gravity, float force_x, float force_y, float limit_x, float limit_y) {
    /* Initialize the System */
    this->system = phy::System(gravity, force_x, force_y, limit_x, limit_y);

    /* Setup Renderer Settings */
    this->name = p_name;
    this->paused = false;
    this->enable_inputs = true;

    /* Setup Camera Settings */
    this->camera_zoom = 100.0f;
    this->screen_width = camera_w;
    this->screen_height = camera_h;
    this->camera_x = 0.0f;
    this->camera_y = 0.0f;

    /* Setup Defaults Inputs */
    this->mouse_x = 0;
    this->mouse_y = 0;
    this->sys_mouse_x = 0;
    this->sys_mouse_y = 0;
    this->saved_mouse_pos = sf::Vector2f();
    this->selected_area = {sf::Vector2f(), sf::Vector2f()};

    this->select_type = 0;
    this->debug_type = 0;
    this->counter_debug = 0;

    this->selected_corpses_fixed = {};
    this->selected_corpses_cursor = {};
    this->selected_corpses_diff = {};

    /* Setup Renderer View */
    this->view.reset(sf::FloatRect(-screen_width / 2, -screen_height / 2, this->screen_width, this->screen_height));
    this->Camera(sf::Vector2f(camera_x, camera_y), zoom);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    /* Initialize the Window */
    this->window.create(sf::VideoMode(this->screen_width, this->screen_height), this->name, sf::Style::Default, settings);
    this->window.setView(this->view);
    this->window.setFramerateLimit(60);

    /* Setup the Gui */
    this->SetupGui();
}

Renderer::~Renderer() {}

void Renderer::Render() {
    /* Main Rendering Loop */
    while (this->window.isOpen()) {
        if (!this->Paused()) {
            this->system.Step();
        }

        /* Background Color */
        this->window.clear(G_BACKGROUND_COLOR);

        /* Events Handling */
        sf::Event event;
        while (this->window.pollEvent(event)) {
            Input(event);
        }

        /* Delta Time Clock */
        ImGui::SFML::Update(this->window, this->frame = this->clock.restart());

        /* Draw Elements */
        this->DrawGui();
        this->Draw();
        this->Debug();

        /* Render Elements */
        this->RenderGui();
        this->RenderWindow();
    }

    /* Clean the ImGui processes*/
    ImGui::SFML::Shutdown();
}

void Renderer::RenderWindow() { this->window.display(); }

void Renderer::Close() {
    if (this->window.isOpen()) {
        this->window.close();
    }
}

void Renderer::Pause() { this->paused = !this->paused; }
int Renderer::Framerate() { return (1000 / this->frame.asMilliseconds()); }
void Renderer::UpdateDebug() {
    debug_values[0] = Framerate();
    debug_values[1] = this->debug_type;
    debug_values[2] = this->mouse_x;
    debug_values[3] = this->mouse_y;
    debug_values[4] = this->sys_mouse_x;
    debug_values[5] = this->sys_mouse_y;
    debug_values[6] = ftn::digits_comma(1 / get_camera_size(), 3);
    debug_values[7] = this->camera_x;
    debug_values[8] = this->camera_y;
    debug_values[9] = this->paused;
    debug_values[10] = this->system.get_dt();
    debug_values[11] = this->select_type;
    debug_values[12] = this->system.get_corpses_size();
}

void Renderer::Draw() {
    for (int i = 0; i < system.get_corpses_size(); i++) {
        DrawCorpse(system.get_corpse(i));
    }
    DrawLimits();
}

void Renderer::Camera(sf::Vector2f move, float zoom) {
    this->view.setCenter(this->view.getCenter() + move);
    this->view.zoom(zoom);

    this->camera_x = this->view.getCenter().x;
    this->camera_y = this->view.getCenter().y;
    this->camera_zoom = this->camera_zoom * zoom;

    this->window.setView(this->view);
}

bool Renderer::Paused() { return this->paused; }

float Renderer::get_mouse_x() { return this->mouse_x; }
float Renderer::get_mouse_y() { return this->mouse_y; }

int Renderer::get_select_type() { return this->select_type; }
int Renderer::get_debug_type() { return this->debug_type; }
void Renderer::set_debug_type(int i) { this->debug_type = i; }

float Renderer::get_camera_x() { return this->camera_x; }
float Renderer::get_camera_y() { return this->camera_y; }

void Renderer::set_camera_x(float camera_x) { this->camera_x = camera_x; }
void Renderer::set_camera_y(float camera_y) { this->camera_y = camera_y; }

float Renderer::get_camera_zoom() { return this->camera_zoom; }
float Renderer::get_camera_size() { return this->camera_zoom / 100.0f; }
void Renderer::set_camera_zoom(float camera_zoom) { this->camera_zoom = camera_zoom; }

sf::Vector2f Renderer::get_real_pos(sf::Vector2i pos) {
    return window.mapPixelToCoords(pos);
    // return sf::Vector2f(get_real_pos_x(pos.x), get_real_pos_y(pos.y));
}
float Renderer::get_real_pos_x(float x) {
    return window.mapPixelToCoords(sf::Vector2i(x, 0)).x;
    // return this->view.getCenter().x + (this->camera_x + x -
    // this->view.getCenter().x - (this->screen_width/2)) * get_camera_size();
}
float Renderer::get_real_pos_y(float y) {
    return window.mapPixelToCoords(sf::Vector2i(0, y)).y;
    // return this->view.getCenter().y + (this->camera_y + y -
    // this->view.getCenter().y - (this->screen_height/2)) * get_camera_size();
}

bool Renderer::rect_in_screen(ftn::Rectangle rect) {
    // One point in screen
    if (rect.pos.x > get_real_pos_x(0) && rect.pos.x < get_real_pos_x(screen_width)) {
        return true;
    }
    if (rect.pos.x + rect.size.x > get_real_pos_x(0) && rect.pos.x + rect.size.x < get_real_pos_x(screen_width)) {
        return true;
    }
    if (rect.pos.y > get_real_pos_y(0) && rect.pos.y < get_real_pos_y(screen_height)) {
        return true;
    }
    if (rect.pos.y + rect.size.y > get_real_pos_y(0) && rect.pos.y + rect.size.y < get_real_pos_y(screen_height)) {
        return true;
    }

    // Or screen in the shape
    if (rect.pos.x < get_real_pos_x(0) && rect.pos.x + rect.size.x > get_real_pos_x(screen_width) && rect.pos.y < get_real_pos_y(0) && rect.pos.y + rect.size.y > get_real_pos_y(screen_height)) {
        return true;
    }

    return false;  // is it faster to test first for true or for false?
}

void Renderer::addText(ftn::Text txt) { this->texts.push_back(txt); }
void Renderer::DrawTexts() {
    for (int i = 0; i < this->texts.size(); i++) {
        ftn::Text txt = this->texts.at(i);
        DrawText(txt.str, txt.x, txt.y, txt.size, txt.fixed, txt.color);
    }
}

bool Renderer::get_enable_inputs() { return this->enable_inputs; }

void Renderer::set_enable_inputs(bool enable) { this->enable_inputs = enable; }
