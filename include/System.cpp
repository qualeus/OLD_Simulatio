#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <math.h>

#include "Body/Dot.cpp"
#include "Constraint/Constraint.cpp"

using namespace std;
using namespace sf;

class System
{
private:
	string name;
	float forceX;
	float forceY;
	bool gravity;

	vector<Dot> Corpses;
	vector<Constraint> Constraints;
	vector<pair<Dot*, Dot*>> Pairs;
	
	Dot* sDot = nullptr;
	Dot saveDot = Dot();

	int sType;
	int debugType;
	bool paused;
	float mouseX;
	float mouseY;

	RenderWindow renderer;
	View view;
	Clock clock;
	Time frameTime;

	float camZoom;
	float camX;
	float camY;


	void Initialize(string name, bool gravity, float forceX, float forceY, int screenX, int screenY)
	{
		//CONFIG
		this->name = name;
		this->gravity = gravity;
		this->forceX = forceX;
		this->forceY = forceY;
		this->Corpses = vector<Dot>();

		//DEBUG
		this->sType = 0;
		this->debugType = 0;
		this->paused = false;

		//CAMERA
		this->camZoom = 100.0f;
		this->camX = 0.0f;
		this->camY = 0.0f;

		this->view.reset(FloatRect(this->camX, this->camY, this->camX + screenX, this->camY + screenY));

		//RENDERER
		ContextSettings settings;
		settings.antialiasingLevel = 8;
		
		this->renderer.create(VideoMode(screenX, screenY), this->name, Style::Default, settings);
		this->renderer.setView(this->view);
		this->renderer.setFramerateLimit(60);
	}

public:
	System()
	{
		Initialize("Default", false, 0.0f, 0.0f, 600, 400);
	}

	System(string name)
	{
		Initialize(name, false, 0.0f, 0.0f, 600, 400);
	}

	System(string name, bool gravity, float forceX, float forceY)
	{
		Initialize(name, gravity, forceX, forceY, 600, 400);
	}

	System(string name, bool gravity, float forceX, float forceY, int screenX, int screenY)
	{
		Initialize(name, gravity, forceX, forceY, screenX, screenY);
	}

	void addDot(Dot b)
	{
		this->Corpses.emplace_back(b);
	}

	void addConstraint(Constraint c)
	{
		this->Constraints.emplace_back(c);
	}

	Dot* getDot(int index)
	{
		if (index >= 0 && index < this->Corpses.size())
		{
			return &this->Corpses.at(index);
		}
		else
		{
			return nullptr;
		}
	}

	Constraint* getConstraint(int index)
	{
		if (index >= 0 && index < this->Constraints.size())
		{
			return &this->Constraints.at(index);
		}
		else
		{
			return nullptr;
		}
	}

	void Prepare()
	{
		for (int x = 0; x < this->Corpses.size(); x++)
		{
			for (int y = 0; y < this->Corpses.size(); y++)
			{
				bool exist = false;

				for (int z = 0; z < Pairs.size(); z++)
				{
					Dot a = *Pairs.at(z).first;
					Dot b = *Pairs.at(z).second;

					if (a.Equals(Corpses.at(x)) && b.Equals(Corpses.at(y)) || a.Equals(Corpses.at(y)) && b.Equals(Corpses.at(x)))
					{
						exist = true;
					}
				}				

				if (exist == false)
				{
					if (this->Corpses.at(x).Equals(this->Corpses.at(y)) != true)
					{
						Pairs.push_back({&this->Corpses.at(x), &this->Corpses.at(y)});
					}
				}
			}
		}
	}

	void Forces()
	{
		for (int z = 0; z < Pairs.size(); z++)
		{
			Dot& DotA = *Pairs.at(z).first;
			Dot& DotB = *Pairs.at(z).second;

			float distance = sqrt(pow(DotA.getX() - DotB.getX(), 2) + pow(DotA.getY() - DotB.getY(), 2));

			if (distance < DotA.getSize() + DotB.getSize() && !DotA.isEtherial() && !DotB.isEtherial())
			{
				Collision(DotA, DotB);
			}
			
			if (this->gravity)
			{
				Gravity(DotA, DotB);
			}
						
		}

		for (int z = 0; z < Corpses.size(); z++)
		{
			Dot& b = Corpses.at(z);
			b.setspdX(b.getspdX() + this->forceX);
			b.setspdY(b.getspdY() + this->forceY);
		}
	}

	void Collision(Dot& DotA, Dot& DotB)
	{
		float distanceX = DotA.getX() - DotB.getX();
		float distanceY = DotA.getY() - DotB.getY();
		float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
		float overlap = 0.5f * (distance - DotA.getSize() - DotB.getSize());
		
		
		if (DotA.isFixed() || DotB.isFixed())
		{
			if (DotA.isFixed() && DotB.isFixed())
			{
				DotB.setX(DotB.getX() + 2 * overlap * (DotA.getX() - DotB.getX()) / distance);
				DotB.setY(DotB.getY() + 2 * overlap * (DotA.getY() - DotB.getY()) / distance);
			}
			else
			{
				if (!DotA.isFixed())
				{
					DotA.setX(DotA.getX() - 2 * overlap * (DotA.getX() - DotB.getX()) / distance);
					DotA.setY(DotA.getY() - 2 * overlap * (DotA.getY() - DotB.getY()) / distance);
				}
				else
				{
					DotB.setX(DotB.getX() + 2 * overlap * (DotA.getX() - DotB.getX()) / distance);
					DotB.setY(DotB.getY() + 2 * overlap * (DotA.getY() - DotB.getY()) / distance);
				}
			}
		}
		else
		{
			DotA.setX(DotA.getX() - overlap * (DotA.getX() - DotB.getX()) / distance);
			DotA.setY(DotA.getY() - overlap * (DotA.getY() - DotB.getY()) / distance);
		
			DotB.setX(DotB.getX() + overlap * (DotA.getX() - DotB.getX()) / distance);
			DotB.setY(DotB.getY() + overlap * (DotA.getY() - DotB.getY()) / distance);
		}
		
		distanceX = DotA.getX() - DotB.getX();
		distanceY = DotA.getY() - DotB.getY();
		distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
		
		float normalX = (DotB.getX() - DotA.getX()) / distance;
		float normalY = (DotB.getY() - DotA.getY()) / distance;

		float tangentX = -normalY;
		float tangentY = normalX;

		float dpTanA = DotA.getspdX() * tangentX + DotA.getspdY() * tangentY;
		float dpTanB = DotB.getspdX() * tangentX + DotB.getspdY() * tangentY;
		
		float dpNormA = DotA.getspdX() * normalX + DotA.getspdY() * normalY;
		float dpNormB = DotB.getspdX() * normalX + DotB.getspdY() * normalY;

		float mA = (dpNormA * (DotA.getMass() - DotB.getMass()) + 2.0f * DotB.getMass() * dpNormB) / (DotA.getMass() + DotB.getMass());
		float mB = (dpNormB * (DotB.getMass() - DotA.getMass()) + 2.0f * DotA.getMass() * dpNormA) / (DotA.getMass() + DotB.getMass());

		if (DotA.isFixed() || DotB.isFixed())
		{
			if (!DotA.isFixed())
			{
				DotA.setspdX(tangentX * dpTanA + normalX * mA);
				DotA.setspdY(tangentY * dpTanA + normalY * mA);
			}
			else
			{
				DotB.setspdX(tangentX * dpTanB + normalX * mB);
				DotB.setspdY(tangentY * dpTanB + normalY * mB);
			}
		}
		else
		{
			DotA.setspdX(tangentX * dpTanA + normalX * mA);
			DotA.setspdY(tangentY * dpTanA + normalY * mA);
		
			DotB.setspdX(tangentX * dpTanB + normalX * mB);
			DotB.setspdY(tangentY * dpTanB + normalY * mB);
		}
	
	}

	void Gravity(Dot& DotA, Dot& DotB)
	{
		float dist = sqrt(pow(DotA.getX() - DotB.getX(), 2) + pow(DotA.getY() - DotB.getY(), 2));
		double attr = 9.81 * ((DotA.getMass() * DotB.getMass()) / pow(dist, 2));

		if (!DotA.isFixed())
		{
			DotA.setspdX(DotA.getspdX() + attr * ((DotB.getX() - DotA.getX()) / dist));
			DotA.setspdY(DotA.getspdY() + attr * ((DotB.getY() - DotA.getY()) / dist));
		}
		
		if (!DotB.isFixed())
		{
			DotB.setspdX(DotB.getspdX() - attr * ((DotB.getX() - DotA.getX()) / dist));
			DotB.setspdY(DotB.getspdY() - attr * ((DotB.getX() - DotA.getX()) / dist));
		}

		if (debugType > 1)
		{
			DrawLine(DotA.getX(), DotA.getY(), DotA.getX() + 20 * attr * ((DotB.getX() - DotA.getX()) / dist), DotA.getY() + 20 * attr * ((DotB.getY() - DotA.getY()) / dist), Color::Blue);
			DrawLine(DotB.getX(), DotB.getY(), DotB.getX() - 20 * attr * ((DotB.getX() - DotA.getX()) / dist), DotB.getY() - 20 * attr * ((DotB.getY() - DotA.getY()) / dist), Color::Blue);
		}
	}

	void Input(Event event)
	{
		if (event.type == Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left || event.mouseButton.button == Mouse::Right)
			{
				if (this->sDot != nullptr)
				{
					sDot->setFixed(saveDot.isFixed());
					this->saveDot = Dot();
					this->sDot = nullptr;
				}

				for (int x = 0; x < this->Corpses.size(); x++)
				{
					if (PointDot(this->mouseX, this->mouseY, this->Corpses.at(x)))
					{
						sDot = &this->Corpses.at(x);
						saveDot = *sDot;

						sDot->setFixed(true);
						sDot->setspdX(0);
						sDot->setspdY(0);

						if (event.mouseButton.button == Mouse::Left)
						{
							sType = 1;
						}
						else if (event.mouseButton.button == Mouse::Right)
						{
							sType = 2;
						}
						break;
					}
				}
			}
		}
		
		if (event.type == Event::MouseMoved)
		{
			Vector2i mousePosition = Mouse::getPosition(this->renderer);
			this->mouseX = mousePosition.x + this->camX;
			this->mouseY = mousePosition.y + this->camY;

			if (sDot != nullptr)
			{
				if (sType == 1)
				{
					sDot->setX(this->mouseX);
					sDot->setY(this->mouseY);
				}
				
			}
		}
		
		if (event.type == Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == Mouse::Left || event.mouseButton.button == Mouse::Right)
			{
				if (sType == 2)
				{
					sDot->setspdX(0.1 * (sDot->getX() - this->mouseX));
					sDot->setspdY(0.1 * (sDot->getY() - this->mouseY));
				}

				sType = 0;
				if (this->sDot != nullptr)
				{
					sDot->setFixed(saveDot.isFixed());
					this->saveDot = Dot();
					this->sDot = nullptr;
				}
			}
		}

		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::D)
			{
				if (this->debugType < 3)
				{
					this->debugType += 1;
				}
				else
				{
					this->debugType = 0;
				}
			}

			if (event.key.code == Keyboard::A)
			{
				camUpdate(-10,0);
			}
			if (event.key.code == Keyboard::Z)
			{
				camUpdate(0,-10);
			}
			if (event.key.code == Keyboard::E)
			{
				camUpdate(10,0);
			}
			if (event.key.code == Keyboard::S)
			{
				camUpdate(0,10);
			}

			if (event.key.code == Keyboard::W)
			{
				camUpdate(0, 10, 0.9f);
			}

			if (event.key.code == Keyboard::X)
			{
				camUpdate(0, 0, 1.1f);
			}

			if (event.key.code == Keyboard::Space)
			{
				this->paused = !this->paused;
			}

		}
	}

	bool PointDot(int x, int y, Dot Dot)
	{
		float dist = sqrt(pow(x - Dot.getX(), 2) + pow(y - Dot.getY(), 2));
		return (dist <= Dot.getSize());
	}

	void Move()
	{
		if (!this->paused)
		{
			this->Forces();
			
			for (int i = 0; i < this->Corpses.size(); i++)
			{
				this->getDot(i)->Move();
			}

			for (int x = 0; x < 10; x++)
			{
				for (int i = 0; i < this->Constraints.size(); i++)
				{
					this->getConstraint(i)->Move();
				}
			}
		}
	}

	void Draw()
	{
		for (int i = 0; i < this->Corpses.size(); i++)
		{
			Dot b = *this->getDot(i);
			DrawCircle(b.getX(),b.getY(),b.getSize(),b.getColor());
		}

		for (int i = 0; i < this->Constraints.size(); i++)
		{
			Constraint c = *this->getConstraint(i);
			Dot& d1 = *c.getD1();
			Dot& d2 = *c.getD2();
			DrawLine(d1.getX(), d1.getY(), d2.getX(), d2.getY(), Color::Green);
		}
	}

	void Debug()
	{
		
		this->frameTime = clock.restart();
		int framerate = 1 / (frameTime.asMilliseconds() * 0.001);
		DrawText(to_string(framerate), camX + 0, camY + 0, 30, Color::Yellow);

		DrawText("x: " + to_string(this->camX), camX + 0, camY + 30, 20, Color::Yellow);
		DrawText("y: " + to_string(this->camY), camX + 0, camY + 50, 20, Color::Yellow);
		DrawText("z: " + to_string(this->camZoom), camX + 0, camY + 70, 20, Color::Yellow);
		DrawText("paused: " + to_string(this->paused), camX + 0, camY + 90, 20, Color::Yellow);
		
		DrawText("[D] Debug: " + to_string(debugType),camX + this->renderer.getSize().x - 150, camY + 0, 24, Color::Yellow);

		for (int i = 0; i < this->Corpses.size(); i++)
		{
			Dot b = *this->getDot(i);
			
			if (debugType > 0)
			{
				DrawLine(b.getX(), b.getY(), b.getX() + 10 * b.getspdX(), b.getY() + 10 * b.getspdY(), Color::Red);
			}
		}

		for (int i = 0; i < Pairs.size(); i++)
		{
			Dot a = *Pairs.at(i).first;
			Dot b = *Pairs.at(i).second;
			
			float dist = sqrt(pow(a.getX() - b.getX(), 2) + pow(a.getY() - b.getY(), 2));

			if (dist > a.getSize() + b.getSize() + 1)
			{
				if (debugType > 2)
				{
					DrawLine(a.getX(), a.getY(), b.getX(), b.getY(), Color::White);
				}
			}
			else
			{
				if (debugType > 1)
				{	
					DrawLine(a.getX(), a.getY(), b.getX(), b.getY(), Color::Red);
				}
			}
		}

		if (sDot != nullptr)
		{
			if (sType == 2)
			{
				DrawLine(sDot->getX(), sDot->getY(), mouseX, mouseY, Color::Blue);
			}
		}
	}

	void Render()
	{
		Prepare();
		while (this->renderer.isOpen())
		{

			this->renderer.clear(sf::Color::Black);

			Event event;
			while (this->renderer.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					this->renderer.close();
				}
				else
				{
					Input(event);
				}
			}

			this->Move();
			this->Draw();
			this->Debug();
			this->renderer.display();
		}
	}

	void DrawLine(int x1, int y1, int x2, int y2, Color color = Color::White)
	{
		float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
		RectangleShape line(Vector2f(dist, 5));
		line.setOrigin(0, 2);
		line.setPosition(x2, y2);
		line.rotate(atan2(y1 - y2, x1 - x2) * (180 / PI));
		line.setFillColor(color);
		this->renderer.draw(line);
	}

	void DrawCircle(int x, int y, int radius, Color color = Color::White)
	{
		CircleShape circle(radius);
		circle.setPosition(x, y);
		circle.setFillColor(color);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		this->renderer.draw(circle);
	}

	void DrawText(string str, int x = 0, int y = 0, int size = 20, Color color = Color::White)
	{
		Font font;
		Text text;

		if (font.loadFromFile("resource/arial.ttf"))
		{
			text.setPosition(x, y);
			text.setFont(font);
			text.setString(str);
			text.setCharacterSize(size);
			text.setFillColor(color);
			this->renderer.draw(text);
		}		
	}

	template <typename T>
	std::string to_string(T value)
	{
	    std::ostringstream oss;
	    oss << value;
	    return oss.str();
	}

	void camUpdate(float X, float Y, float Z = 1.0f)
	{
		this->camX = this->camX + X;
		this->camY = this->camY + Y;
		this->camZoom = this->camZoom * Z;

		this->view.move(X, Y);
		this->view.zoom(Z);
		this->renderer.setView(this->view);
	}
};