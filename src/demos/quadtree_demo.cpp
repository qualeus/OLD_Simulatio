#include <iostream>

#include "../../include/Renderer.hpp"

int main()
{

	Renderer demo = Renderer(350.0f, 250.0f, 800.0f, 1200.0f, 0.7f, "Quadtree Demo", false, 0.0f, 0.0f, 1000, 1000);
	demo.set_debug_type(6);
	
	demo.addText({"Move the red ball to see the \ncollision detection pairs update.", 265, 100, 15, false, C_SUN});
	demo.addText({"On a edge, it associate with every body of \nthe sub node (where the edge is crossed).", 390, 240, 15, false, C_SUN});
	demo.addText({"Place it here and it will not \nhave to check any collision.", 290, 360, 15, false, C_SUN});
	demo.addText({"Here, it will associate with the \ntwo bodies to check if it collide.", 25, 380, 15, false, C_SUN});
	demo.addText({"There, it will overflow the capacity \nof the edge and subdivise it.", 20, 40, 15, false, C_SUN});
	demo.addText({"The corpses out the grid are soft \ndestroyed for better performances.", 510, 360, 15, false, C_SUN});
	
	demo.system.addCorpse(phy::Circle(170, 120, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_TURQUOISE));
	demo.system.addCorpse(phy::Circle(30, 160, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_TURQUOISE));
	demo.system.addCorpse(phy::Circle(80, 160, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_TURQUOISE));
	demo.system.addCorpse(phy::Circle(210, 170, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_TURQUOISE));
	demo.system.addCorpse(phy::Circle(150, 200, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_TURQUOISE));

	demo.system.addCorpse(phy::Circle(360, 260, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_RED));

	demo.system.addCorpse(phy::Circle(180, 300, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_RIVER));
	demo.system.addCorpse(phy::Circle(130, 330, 20, 5.0f, 2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, C_RIVER));

	demo.Render();
 	return 0;
}