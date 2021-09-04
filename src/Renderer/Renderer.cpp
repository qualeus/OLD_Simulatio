
#include "../../include/Renderer/Renderer.hpp"

Renderer::Renderer(float camera_x, float camera_y, float camera_h, float camera_w, float zoom, std::string p_name, bool gravity, float force_x, float force_y, float limit_x, float limit_y, int quadtree_max_count, int quadtree_max_depth)
    : system(gravity, force_x, force_y, limit_x, limit_y, quadtree_max_count, quadtree_max_depth) {
    /* Initialize the System */
    // this->system = phy::System(gravity, force_x, force_y, limit_x, limit_y);

    /* Setup Renderer Settings */
    this->name = p_name;
    this->paused = false;
    this->enable_inputs = true;

    /* Setup Camera Settings */
    this->camera_zoom = 100.0f;
    this->screen_width = static_cast<int>(camera_w);
    this->screen_height = static_cast<int>(camera_h);
    this->camera_x = 0.0f;
    this->camera_y = 0.0f;

    /* Setup Defaults Inputs */
    this->mouse_x = 0;
    this->mouse_y = 0;
    this->sys_mouse_x = 0;
    this->sys_mouse_y = 0;
    this->saved_mouse_pos = sf::Vector2f();
    this->selected_area = gmt::Bounds<float>();

    this->select_type = 0;
    this->debug_type = 0;
    this->counter_debug = 0;

    this->selected_corpses_fixed = {};
    this->selected_corpses_cursor = {};
    this->selected_corpses_index = {};
    this->selected_corpses_diff = {};

    this->input_spawner = {};
    this->spawners = {};
    this->corpses_colors = {};

    /* Setup Renderer View */
    this->view.reset(sf::FloatRect(-screen_width / 2, -screen_height / 2, this->screen_width, this->screen_height));
    this->Camera(sf::Vector2f(camera_x, camera_y), std::max(zoom, 0.01f));

    this->vertices_buffer = {};

    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;

    /* Initialize the Window */
    this->window.create(sf::VideoMode(this->screen_width, this->screen_height), this->name, sf::Style::Default, settings);
    this->window.setFramerateLimit(max_framerate);

    /* Initialize the RenderTexture*/
    if (!this->render_texture.create(this->screen_width, this->screen_height)) LOG_ERROR("Render texture couldn't be initialized.");
    this->render_texture.setView(this->view);

    /* Setup the Window Icon */
    sf::Image icon;
    icon.loadFromMemory(Ricon_compressed_data, Ricon_compressed_size);
    this->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    /* Setup the Gui */
    this->SetupGui();

    /* Setup the Drawing */
    this->SetupDraw();
}

Renderer::~Renderer() {}

void Renderer::Render() {
    /* Main Rendering Loop */
    while (this->window.isOpen()) {
        /* Reset Benchmarking */
        this->ResetBenchmark();
        {
            /* Base */
            bmk::Record("#");
            /* Frame */
            {
                bmk::Record("Frame");
                if (!this->Paused()) {
                    this->system.Step();
                    this->UpdateCorpseDatas();
                    this->UpdateSpawners();
                }

                /* Background Color */
                this->window.clear(background_color);
                this->render_texture.clear(background_color);

                /* Events Handling */
                sf::Event event;
                while (this->window.pollEvent(event)) { Input(event); }
                this->UpdateCamera();

                /* Delta Time Clock */
                ImGui::SFML::Update(this->window, this->frame = this->clock.restart());

                /* Draw Elements */
                {
                    bmk::Record("Drawing");
                    {
                        bmk::Record("Gui");
                        this->DrawGui();
                    }
                    {
                        bmk::Record("System");
                        this->Draw();
                    }
                    {
                        bmk::Record("Debug");
                        this->Debug();
                    }
                }

                /* Render Elements */
                this->RenderTexture();
                this->RenderGui();
                this->RenderWindow();
            }
        }
    }

    /* Clean the ImGui processes*/
    ImGui::SFML::Shutdown();
}
void Renderer::ResetBenchmark() {
    if (benchmark_count++ > benchmark_reset || !benchmark_recording) {
        bmk::Recorder::root.Reset();
        benchmark_count = 0;
    }
    if (benchmark_paused) {
        if (bmk::Recorder::root.childs.size() > 0) { bmk::Recorder::root.childs.pop_back(); }
    }
}

void Renderer::RenderTexture() {
    // We finalize the render texture computation
    this->render_texture.display();

    // Draw the texture on a Sprite
    this->render_sprite.setTexture(this->render_texture.getTexture(), true);
    // this->render_sprite.setTextureRect(sf::IntRect(0, 0, this->get_screen_width(), this->get_screen_height()));
    // this->render_sprite.setPosition(-this->get_screen_width() / 2.0f, -this->get_screen_height() / 2.0f);
    // this->render_sprite.setOrigin(this->get_screen_width() / 2.0f, this->get_screen_height() / 2.0f);

    // ...then we apply it on the window
    blur_shader.setUniform("texture", this->render_texture.getTexture());
    this->window.draw(this->render_sprite, &blur_shader);
}

void Renderer::RenderWindow() {
    // We compute the windows display
    this->window.display();
}

void Renderer::Close() {
    if (this->window.isOpen()) { this->window.close(); }
}

void Renderer::Pause() { this->paused = !this->paused; }
void Renderer::UpdateMaxFramerate(int max_framerate) {
    this->max_framerate = max_framerate;
    this->window.setFramerateLimit(max_framerate);
}

int Renderer::Framerate() { return (1000 / (this->frame.asMilliseconds() + 0.00001f)); }

void Renderer::UpdateCorpseDatas() {
    // TODO delete colors in the map when object are also deleted
    for (int i = 0; i < this->selected_corpses_cursor.size(); i++) {
        int curr_id = system.get_corpse(this->selected_corpses_cursor[i])->get_id();
        int real_id = this->selected_corpses_index[i];
        console.Log(gmt::to_string(curr_id) + "==" + gmt::to_string(real_id));
        if (curr_id != real_id) {
            this->select_type = S_DEFAULT;
            this->selected_corpses_cursor = {};
            this->selected_corpses_index = {};
            this->selected_corpses_fixed = {};
            this->selected_corpses_diff = {};

            /* TODO: repopulate the slections array with the id stored in the array of index when the corpse still exist */
            return;
        }
    }
}

void Renderer::UpdateDebug() {
    debug_values[0] = Framerate();
    debug_values[1] = this->debug_type;
    debug_values[2] = this->mouse_x;
    debug_values[3] = this->mouse_y;
    debug_values[4] = this->sys_mouse_x;
    debug_values[5] = this->sys_mouse_y;
    debug_values[6] = gmt::digits_comma(1 / get_camera_size(), 3);
    debug_values[7] = this->camera_x;
    debug_values[8] = this->camera_y;
    debug_values[9] = this->paused;
    debug_values[10] = this->system.get_dt();
    debug_values[11] = this->select_type;
    debug_values[12] = this->system.get_corpses_size();
    debug_values[13] = this->system.get_pairs_size();
    debug_values[14] = this->system.get_quad_pairs_size();
    debug_values[15] = this->system.get_quad_pairs_depth();
    debug_values[16] = this->system.get_quad_pairs_size(0);
    debug_values[17] = this->collision_number / DELAY_DEBUG;
}

void Renderer::Camera(sf::Vector2f move, float zoom) {
    this->set_camera_x(this->view.getCenter().x + move.x);
    this->set_camera_y(this->view.getCenter().y + move.y);
    this->camera_zoom = this->camera_zoom * zoom;
}

void Renderer::UpdateCamera() {
    this->view.setCenter(get_camera_x(), get_camera_y());
    this->view.setSize(get_screen_width() * get_camera_size(), get_screen_height() * get_camera_size());
    this->render_texture.setView(this->view);
    this->window.setSize({static_cast<unsigned int>(get_screen_width()), static_cast<unsigned int>(get_screen_height())});
    this->window.setFramerateLimit(max_framerate);
}

bool Renderer::Paused() { return this->paused; }

void Renderer::addCorpse(phy::Polygon polygon, sf::Color color) {
    this->corpses_colors[polygon.get_id()] = color;
    this->system.addCorpse(polygon);
}

void Renderer::addCorpse(phy::Circle circle, sf::Color color) {
    this->corpses_colors[circle.get_id()] = color;
    this->system.addCorpse(circle);
}

std::shared_ptr<phy::Corpse> Renderer::getCorpse(int index) const { return this->system.get_corpse(index); }

void Renderer::addConstraint(phy::Link link, sf::Color color) {
    this->constraints_colors[link.get_id()] = color;
    this->system.addConstraint(link);
}

void Renderer::addConstraint(phy::Spring spring, sf::Color color) {
    this->constraints_colors[spring.get_id()] = color;
    this->system.addConstraint(spring);
}

void Renderer::addConstraint(phy::Slider slider, sf::Color color) {
    this->constraints_colors[slider.get_id()] = color;
    this->system.addConstraint(slider);
}

std::shared_ptr<phy::Constraint> Renderer::getConstraint(int index) const { return this->system.get_constraint(index); }

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
void Renderer::set_camera_zoom(float camera_zoom) { this->camera_zoom = camera_zoom; }

float Renderer::get_camera_size() { return this->camera_zoom / 100.0f; }
void Renderer::set_camera_size(float camera_size) { this->camera_zoom = camera_size * 100.0f; }

int Renderer::get_screen_width() { return this->screen_width; }
void Renderer::set_screen_width(int screen_width) { this->screen_width = screen_width; }

int Renderer::get_screen_height() { return this->screen_height; }
void Renderer::set_screen_height(int screen_height) { this->screen_height = screen_height; }

int Renderer::get_max_framerate() { return this->max_framerate; }
void Renderer::set_max_framerate(int max_framerate) { this->max_framerate = max_framerate; }

gmt::Bounds<float> Renderer::get_screen_bounds() { return gmt::Bounds<float>(get_real_pos_x(0), get_real_pos_y(0), get_real_pos_x(screen_width * PIXEL_SCALE), get_real_pos_y(screen_height * PIXEL_SCALE)); }

sf::Vector2f Renderer::get_real_pos(sf::Vector2i pos) { return render_texture.mapPixelToCoords(sf::Vector2i(pos.x, pos.y)) / PIXEL_SCALE; }
float Renderer::get_real_pos_x(float x) { return render_texture.mapPixelToCoords(sf::Vector2i(x, 0)).x / PIXEL_SCALE; }
float Renderer::get_real_pos_y(float y) { return render_texture.mapPixelToCoords(sf::Vector2i(0, y)).y / PIXEL_SCALE; }

bool Renderer::get_enable_inputs() { return this->enable_inputs; }
void Renderer::set_enable_inputs(bool enable) { this->enable_inputs = enable; }
