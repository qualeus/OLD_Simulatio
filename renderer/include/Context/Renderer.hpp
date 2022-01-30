#ifndef Renderer_HPP
#define Renderer_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <bx/math.h>

#include <Geometry/Geometry.hpp>
#include <Geometry/Maths.hpp>
#include <Geometry/String.hpp>
#include <Geometry/Vector.hpp>
#include <Structures/System.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

#include "../Drawing/Camera.hpp"
#include "../Drawing/Mesh.hpp"
#include "../Drawing/Shader.hpp"
#include "../Drawing/Shapes.hpp"
#include "../Drawing/Vertex.hpp"
#include "../Overlay/Gui/GuiManager.hpp"
#include "Inputs.hpp"
#include "Window.hpp"

namespace ctx {

class Renderer {
   private:
    Window window;
    ovl::GuiManager overlay;
    drw::Camera camera;
    std::unordered_map<std::string, bgfx::ProgramHandle> shaders;
    std::unordered_map<std::string, drw::Mesh> meshes;

    int debug = 0;
    glm::vec3 mouse_initial_position = glm::vec3();
    glm::vec3 mouse_initial_towards = glm::vec3();

    std::unordered_map<int, uint32_t> corpses_colors;
    std::unordered_map<int, uint32_t> constraints_colors;

   public:
    phy::System system;

    Renderer();
    void Render();
    void Loop();
    void Debug();

    void Inputs();
    void GlobalInputs();
    void DebugInputs();
    void CameraInputs();

    void DrawSystem();

    void DrawCorpse(std::shared_ptr<phy::Corpse> corpse, uint32_t color);
    void DrawConstraint(std::shared_ptr<phy::Constraint> constraint, uint32_t color);

    void DrawCorpseCircle(phy::Circle *circle, uint32_t color);
    void DrawCorpsePolygon(phy::Polygon *polygon, uint32_t color);

    void DrawConstraintLink(phy::Link *link, uint32_t color);
    void DrawConstraintSpring(phy::Spring *spring, uint32_t color);
    void DrawConstraintSlider(phy::Slider *slider, uint32_t color);

    void LoadShaders();
    void UpdateCamera();

    void addCorpse(phy::Polygon polygon, uint32_t color);
    void addCorpse(phy::Circle circle, uint32_t color);
    std::shared_ptr<phy::Corpse> getCorpse(int index) const;

    void addConstraint(phy::Link link, uint32_t color);
    void addConstraint(phy::Spring spring, uint32_t color);
    void addConstraint(phy::Slider slider, uint32_t color);
    std::shared_ptr<phy::Constraint> getConstraint(int index) const;
};

}  // namespace ctx
#endif