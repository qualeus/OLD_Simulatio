#ifndef Renderer_HPP
#define Renderer_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <Structures/System.hpp>
#include <glm/glm.hpp>

#include "Inputs.hpp"
#include "Window.hpp"

namespace ctx {

class Renderer {
   private:
    Window window;
    phy::System system;
    bool show_demo_window = true;
    bool s_showStats = true;

   public:
    Renderer();
    void Render();
    void Loop();
};

}  // namespace ctx
#endif