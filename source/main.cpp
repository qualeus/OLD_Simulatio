#include <SFML/Graphics.hpp>
#include <iostream> 
#include <math.h>


using namespace sf;
using namespace std;

#include "../include/System/Renderer.cpp"


int main()
{
	Circle circle(400.0f, 300.0f, 10.0f, 5.0f);
	Renderer renderer(true, 1.0f, 0.0f);
	renderer.addCorpse(circle);
	renderer.Render();
	
	cout << circle.getClass() << endl;
	cout << renderer.getCorpse(0)->getClass() << endl;
	
	int age;
	cin >> age;

}

