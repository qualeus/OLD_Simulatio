#ifndef Renderer_HPP
#define Renderer_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <Structures/System.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

#include "../Drawing/Camera.hpp"
#include "../Drawing/Shader.hpp"
#include "../Drawing/Shapes.hpp"
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

   public:
    phy::System system;

    Renderer();
    void Render();
    void Loop();
    void Debug();
    void Inputs();
    void LoadShaders();
};

}  // namespace ctx
#endif