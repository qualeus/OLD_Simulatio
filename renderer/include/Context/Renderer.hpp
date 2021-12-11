#ifndef Renderer_HPP
#define Renderer_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <Structures/System.hpp>
#include <glm/glm.hpp>

#include "Inputs.hpp"
#include "Window.hpp"
#include "../Overlay/Gui/GuiManager.hpp"
#include "../Drawing/Camera.hpp"

namespace ctx {

class Renderer {
   private:
    Window window;
    ovl::GuiManager overlay;
    drw::Camera camera;
    phy::System system;

    bool show_demo_window = true;
    bool s_showStats = true;

   public:
    Renderer();
    void Render();
    void Loop();

    void Inputs();
};

}  // namespace ctx
#endif