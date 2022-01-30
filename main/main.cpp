
#include <stdlib.h>

#include <Context/Renderer.hpp>

int main(void) {
    ctx::Renderer renderer = ctx::Renderer();

    std::cout << gmt::to_string(renderer.system.get_corpses()) << std::endl;
    for (int i = 0; i < 100; ++i) {
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        renderer.addCorpse(phy::Circle(i % 10, i + 10, r), 0xffffffff);
    }

    renderer.addCorpse(phy::Polygon({{0, 0}, {10, 10}, {2, -3}, {7, -2}}), 0xff1322ff);

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