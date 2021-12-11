#ifndef Renderer_HPP
#define Renderer_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <Structures/System.hpp>
#include <glm/glm.hpp>

#include "Window.hpp"

namespace rdr {

class Renderer {
   private:
    drw::Window window;
    phy::System system;
    bool show_demo_window = true;

   public:
    Renderer();
    void Render();
    void Loop();
};

}  // namespace rdr
#endif