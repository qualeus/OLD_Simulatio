
#include <Context/Renderer.hpp>

int main(void) {
    ctx::Renderer renderer = ctx::Renderer();
    for (int i = 0; i < 100; ++i) { renderer.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false)); }

    renderer.system.addConstraint(phy::Link(renderer.system.get_corpse(0), renderer.system.get_corpse(1), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 1.0f, 0.3f, -1000.0f, -300.0f));

    // renderer.system.addCorpse(phy::Polygon({{850, -350}, {950, -350}, {950, 750}, {-50, 750}, {-50, -350}, {50, -350}, {50, 650}, {850, 650}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false));

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