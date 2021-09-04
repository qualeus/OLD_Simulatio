
#include <iostream>

#include "../../include/Geometry/Vector.hpp"
#include "../../include/Renderer/Renderer.hpp"
#include "../demos/baloon.cpp"
#include "../demos/base.cpp"
#include "../demos/cloths.cpp"
#include "../demos/constraints.cpp"
#include "../demos/slider.cpp"
#include "../demos/sponge.cpp"
#include "../demos/spring.cpp"

void TestDemo() {
    Renderer test = Renderer(0.0f, 0.0f, 900.0f, 1600.0f, 1.0f, "Test", false, 0.0f, 0.0f, 100000.0f, 100000.0f, 10, 20);
    test.Render();
}

int main() {
    // BaseDemo();
    // GravityDemo();
    //  ConstraintDemo();
    ClothDemo();
    // SliderDemo();
    //  SpringDemo();
    // BaloonDemo();
    // SpongeDemo();
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