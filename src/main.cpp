#include <iostream>

#include "../include/Renderer.hpp"

int main() {
    Renderer test = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Test", false, 0.0f, 1.0f, 2000.0f, 2000.0f);
    test.set_enable_inputs(false);

    for (int i = 0; i < 100; ++i) {
        test.system.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, sf::Color::Blue));
    }

    /* Concave Shape / Simplify?
    test.system.addCorpse(phy::Polygon({{850, -350},{950, -350},{950, 750},{-50, 750},{-50, -350},{50, -350},{50, 650},{850, 650}}, 5, 1, 0.0f, 0.0f, 0.0f, true, false, C_NEPHRITIS));
    */
    test.system.addCorpse(phy::Polygon({{750, 750}, {950, 750}, {950, -350}, {750, -350}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, C_NEPHRITIS));
    test.system.addCorpse(phy::Polygon({{-150, 750}, {50, 750}, {50, -350}, {-150, -350}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, C_NEPHRITIS));
    test.system.addCorpse(phy::Polygon({{-150, 950}, {950, 950}, {950, 750}, {-150, 750}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, C_NEPHRITIS));

    test.system.addCorpse(phy::Polygon({{600, 50}, {720, 90}, {820, -150}, {590, -10}}, 5, 1, 0.0f, 0.0f, 0.0f, -1.0f, true, true, false, sf::Color::Green));
    test.system.addCorpse(phy::Polygon({{0, 0}, {0, 200}, {200, 200}, {200, 0}}, 5, 1, 0.0f, 0.0f, 0.0f, 5.0f, true, true, false, C_CARROT));
    // test.system.addCorpse(phy::Polygon({{0, 0},{200, 200},{0, 200},{200, 0}}, 5, 1, 0.0f, 0.0f, 0.0f, 5.0f, true, true, false, C_CARROT));

    // test.Pause();
    test.Render();

    return 0;
}