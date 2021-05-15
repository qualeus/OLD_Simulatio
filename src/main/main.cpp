
#include <iostream>

#include "../../include/Geometry/Vector.hpp"
#include "../../include/Renderer/Renderer.hpp"

void BaseDemo() {
    Renderer base = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Base Demo", false, 0.0f, 1.0f, 2000.0f, 2000.0f, 10, 10);

    for (int i = 0; i < 100; ++i) { base.addCorpse(phy::Circle(rand() % 500 + 250, rand() % 500, rand() % 20 + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false), C_SUN); }

    base.addCorpse(phy::Polygon({{850, -350}, {950, -350}, {950, 750}, {-50, 750}, {-50, -350}, {50, -350}, {50, 650}, {850, 650}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false), C_EMERALD);

    // base.system.addCorpse(phy::Polygon({{750, 750}, {950, 750}, {950, -350}, {750, -350}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, C_NEPHRITIS));
    // base.system.addCorpse(phy::Polygon({{-150, 750}, {50, 750}, {50, -350}, {-150, -350}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, C_NEPHRITIS));
    // base.system.addCorpse(phy::Polygon({{-150, 950}, {950, 950}, {950, 750}, {-150, 750}}, 5, 1, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, C_NEPHRITIS));

    base.addCorpse(phy::Polygon({{600, 50}, {720, 90}, {820, -150}, {590, -10}}, 5, 1, 0.0f, 0.0f, 0.0f, -1.0f, true, true, false), C_AMETHYST);
    base.addCorpse(phy::Polygon({{0, 0}, {0, 200}, {200, 200}, {200, 0}}, 5, 1, 0.0f, 0.0f, 0.0f, 5.0f, true, true, false), C_CARROT);
    // test.system.addCorpse(phy::Polygon({{0, 0},{200, 200},{0, 200},{200, 0}}, 5, 1, 0.0f, 0.0f, 0.0f, 5.0f, true, true, false, C_CARROT));

    // test.Pause();
    base.Render();
}

void GravityDemo() {
    Renderer gravity = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Gravity", true, 0.0f, 0.0f, 10000.0f, 10000.0f, 10, 10);
    gravity.addCorpse(phy::Circle(0.0f, 0.0f, 100.0f, 1e+6, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false), sf::Color::Red);
    gravity.addCorpse(phy::Circle(500.0f, 0.0f, 20.0f, 5e+1, 2, 0.0f, -5.2f, 0.0f, 0.0f, false, false, false), sf::Color::Blue);
    // gravity.system.addCorpse(phy::Circle(800.0f, 0.0f, 20.0f, 5e+1, 2, 0.0f, -3.2f, 0.0f, 0.0f, false, false, false, sf::Color::Cyan));

    gravity.Render();
}

void ConstraintDemo() {
    Renderer constraint = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Constraint", false, 0.0f, 0.0f, 100000.0f, 100000.0f, 10, 20);
    int number = 10;
    for (int i = 0; i < number; ++i) { constraint.addCorpse(phy::Circle(rand() % (number * 50), rand() % (number * 50), rand() % 20 + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false), C_SUN); }
    constraint.addConstraint(phy::Link(constraint.getCorpse(0), constraint.getCorpse(1), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 0.01, 0, -1000, false), C_SUN);
    constraint.addConstraint(phy::Link(constraint.getCorpse(0), constraint.getCorpse(2), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 1, 0, -1000, false), C_SUN);
    constraint.addConstraint(phy::Link(constraint.getCorpse(1), constraint.getCorpse(2), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 1, 0, -1000, false), C_SUN);
    constraint.Render();
}

void ClothDemo() {
    Renderer cloth = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Cloth", false, 0.0f, 2.0f, 100000.0f, 100000.0f, 10, 20);
    int width = 70;
    int height = 30;
    float spacing_w = 50;
    float spacing_h = 50;
    float pos_x = -500;
    float pos_y = -500;
    float rigidity = 1;
    bool breaking = true;
    float limit_breaking = -1000;
    bool etherial = true;
    int point_size = 30;

    auto cloth_color = [&](int i, int j) {
        std::vector<int> interpolated = gmt::interpolate_array({C_ALIZARIN.r, C_ALIZARIN.g, C_ALIZARIN.b}, {C_AMETHYST.r, C_AMETHYST.g, C_AMETHYST.b}, static_cast<float>(i) / static_cast<float>(height));
        return sf::Color(interpolated[0], interpolated[1], interpolated[2]);
        // return sf::Color(25 * i, 25 * j, 10);
        return C_SUN;
    };

    for (int i = 0; i < width; i++) { cloth.addCorpse(phy::Circle(pos_x + i * spacing_w, pos_y + 0, point_size, 1, 1, 0, 0, 0, 0, true, true, etherial), C_ALIZARIN); }
    for (int i = 1; i < height; i++) {
        for (int j = 0; j < width; j++) { cloth.addCorpse(phy::Circle(pos_x + j * spacing_w, pos_y + i * spacing_h, point_size, 1, 1, 0, 0, 0, 0, false, false, etherial), cloth_color(i, j)); }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width - 1; j++) { cloth.addConstraint(phy::Link(cloth.getCorpse(j + i * width), cloth.getCorpse(j + i * width + 1), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, rigidity, limit_breaking, limit_breaking, breaking), cloth_color(i, j)); }
    }

    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width; j++) { cloth.addConstraint(phy::Link(cloth.getCorpse(j + i * width), cloth.getCorpse(j + (i + 1) * width), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, rigidity, limit_breaking, limit_breaking, breaking), cloth_color(i, j)); }
    }

    cloth.Render();
}

void SpringDemo() {
    Renderer spring = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Spring", false, 0.0f, 0.0f, 100000.0f, 100000.0f, 10, 20);
    int number = 50;
    for (int i = 0; i < number; ++i) { spring.addCorpse(phy::Circle(rand() % (number * 30), rand() % (number * 30), rand() % number + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false), C_SUN); }
    for (int i = 1; i < number; ++i) { spring.addConstraint(phy::Spring(spring.getCorpse(i - 1), spring.getCorpse(i), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 0.1, 30, 0, -1000, false), C_SUN); }
    spring.Render();
}

void SliderDemo() {
    Renderer spring = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Spring", false, 0.0f, 0.0f, 100000.0f, 100000.0f, 10, 20);
    int number = 20;
    spring.addCorpse(phy::Circle(rand() % (number * 30), rand() % (number * 30), rand() % number + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, true, false, false), C_SUN);
    spring.addCorpse(phy::Circle(rand() % (number * 30), rand() % (number * 30), rand() % number + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, true, false, false), C_SUN);
    for (int i = 2; i < number; ++i) { spring.addCorpse(phy::Circle(rand() % (number * 30), rand() % (number * 30), rand() % number + 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false), C_ASPHALT); }
    phy::Slider slider = phy::Slider(spring.getCorpse(0), spring.getCorpse(1), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, 1, 0, {}, {}, {}, {}, {}, false);
    for (int i = 2; i < number; i++) { slider.addCorpse(spring.getCorpse(i), {0, 0}, true, 0, 0); }
    spring.addConstraint(slider, C_SUN);
    spring.Render();
}

void BalloonDemo() {
    Renderer balloon = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Balloon", false, 0.0f, 0.0f, 100000.0f, 100000.0f, 10, 20);
    int number = 20;
    int radius = 400;
    for (int i = 0; i < number; i++) {
        float angle = static_cast<float>(i) * 2.0f * PI / static_cast<float>(number);
        balloon.addCorpse(phy::Circle(std::cos(angle) * radius, std::sin(angle) * radius, 20, 1.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false), C_RED);
    }

    for (int i = 0; i < number; i++) { balloon.addConstraint(phy::Link(balloon.getCorpse(i), balloon.getCorpse((i + 1) % number), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 1, 0, 0, false), C_RED); }

    std::vector<std::pair<int, int>> inside = gmt::non_touching_pairs(gmt::create_vector(0, number, [](int i) { return i++; }));

    for (int i = 0; i < inside.size(); i++) { balloon.addConstraint(phy::Link(balloon.getCorpse(inside.at(i).first), balloon.getCorpse(inside.at(i).second), {0, 0}, {0, 0}, 0, 0, true, true, 0, 0, -1, 0.2, 0, 0, false), sf::Color(255, 255, 255, 10)); }

    balloon.Render();
}

void TestDemo() {
    Renderer test = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Test", false, 0.0f, 0.0f, 100000.0f, 100000.0f, 10, 20);
    test.Render();
}

int main() {
    // BaseDemo();
    // GravityDemo();
    // ConstraintDemo();
    ClothDemo();
    SliderDemo();
    SpringDemo();
    // BalloonDemo();
    // TestDemo();
    return 0;
}

/*
https://github.com/liabru/matter-js/blob/master/src/geometry/Vector.js
http://buildnewgames.com/gamephysics/
http://cplusplus.com/forum/beginner/89388/
https://en.wikipedia.org/wiki/Verlet_integration
https://compphys.go.ro/newtonian-gravity/
https://femto-physique.fr/omp/methode-de-verlet.php
https://python.plainenglish.io/molecular-dynamics-velocity-verlet-integration-with-python-5ae66b63a8fd


Géometrie
http://paulbourke.net/geometry/polygonmesh/
http://paulbourke.net/geometry/
https://softologyblog.wordpress.com/
http://geomalgorithms.com/a10-_hull-1.html


Quadtree / TRiangulation
http://zenpandainteractive.blogspot.com/2013/03/yet-another-quad-tree-tutorial.html#more
http://zenpandainteractive.blogspot.com/2016/03/the-problem-opengl-and-other-low-level.html#more
*/