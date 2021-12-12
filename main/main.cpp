
#include <Context/Renderer.hpp>

int main(void) {
    ctx::Renderer renderer = ctx::Renderer();
    renderer.system.addCorpse(phy::Circle(10.0f, 5.0f));
    renderer.system.set_gravity(false);
    std::cout << gmt::to_string(renderer.system.get_corpses()) << std::endl;
    renderer.Render();

    // https://dev.to/pperon/hello-bgfx-4dka
    // exemples: https://github.com/podgorskiy/KeplerOrbits
    // + web demo

    // https://github.com/bkaradzic/bgfx/blob/master/examples/common/imgui/imgui.cpp
    // https://github.com/Josh4428/bigg
    // https://github.com/pr0g/sdl-bgfx-imgui-starter/blob/main/main.cpp
    // https://github.com/pr0g/sdl-bgfx-imgui-as_1d-nonlinear-transformations/blob/main/main.cpp
    // https://github.com/bkaradzic/bgfx/blob/master/examples/00-helloworld/helloworld.cpp
    // https://github.com/bkaradzic/bgfx/tree/master/examples/common/imgui

    return 0;
}