#ifndef Renderer_HPP
#define Renderer_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <Structures/System.hpp>
#include <glm/glm.hpp>

#include "../Drawing/Camera.hpp"
#include "../Overlay/Gui/GuiManager.hpp"
#include "Inputs.hpp"
#include "Window.hpp"

namespace ctx {

class Renderer {
   private:
    Window window;
    ovl::GuiManager overlay;
    drw::Camera camera;

    int debug = 0;

   public:
    phy::System system;

    Renderer();
    void Render();
    void Loop();
    void Debug();
    void Inputs();
};

}  // namespace ctx
#endif