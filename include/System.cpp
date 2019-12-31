#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include "Body.cpp"
#include <math.h>

using namespace std;
using namespace sf;

class System
{
private:
	string name;
	float forceX;
	float forceY;
	vector<Body> Corpses;
	RenderWindow renderer;
	vector<pair<Body*, Body*>> Pairs;
	
	Body* sBody = nullptr;
	int sType = 0;
	int debugType = 0;
	float mouseX;
	float mouseY;

	Clock clock;
	Time frameTime;

	bool gravity;

	void Initialize(string name, bool gravity, float forceX, float forceY, int screenX, int screenY)
	{
		this->name = name;
		this->gravity = gravity;
		this->forceX = forceX;
		this->forceY = forceY;

		//Defaults
		this->Corpses = vector<Body>();

		ContextSettings settings;
		settings.antialiasingLevel = 8;
		this->renderer.create(VideoMode(screenX, screenY), this->name, Style::Default, settings);
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

	void add(Body b)
	{
		this->Corpses.emplace_back(b);
	}

	Body* get(int index)
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

	void Prepare()
	{
		for (int x = 0; x < this->Corpses.size(); x++)
		{
			for (int y = 0; y < this->Corpses.size(); y++)
			{
				bool exist = false;

				for (int z = 0; z < Pairs.size(); z++)
				{
					Body a = *Pairs.at(z).first;
					Body b = *Pairs.at(z).second;

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
			Body& bodyA = *Pairs.at(z).first;
			Body& bodyB = *Pairs.at(z).second;

			float distance = sqrt(pow(bodyA.getX() - bodyB.getX(), 2) + pow(bodyA.getY() - bodyB.getY(), 2));

			if (distance < bodyA.getSize() + bodyB.getSize() && !bodyA.isEtherial() && !bodyB.isEtherial())
			{
				Collision(bodyA, bodyB);
			}
			
			if (this->gravity)
			{
				Gravity(bodyA, bodyB);
			}
						
		}

		for (int z = 0; z < Corpses.size(); z++)
		{
			Body& b = Corpses.at(z);
			b.setspdX(b.getspdX() + this->forceX);
			b.setspdY(b.getspdY() + this->forceY);
		}
	}

	void Collision(Body& bodyA, Body& bodyB)
	{
		float distanceX = bodyA.getX() - bodyB.getX();
		float distanceY = bodyA.getY() - bodyB.getY();
		float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
		float overlap = 0.5f * (distance - bodyA.getSize() - bodyB.getSize());
		
		
		if (sBody != nullptr)
		{
			
			if (!sBody->Equals(bodyA) && !bodyA.isFixed())
			{
				bodyA.setX(bodyA.getX() - overlap * (bodyA.getX() - bodyB.getX()) / distance);
				bodyA.setY(bodyA.getY() - overlap * (bodyA.getY() - bodyB.getY()) / distance);
			}

			if (!sBody->Equals(bodyB) && !bodyB.isFixed())
			{
				bodyB.setX(bodyB.getX() + overlap * (bodyA.getX() - bodyB.getX()) / distance);
				bodyB.setY(bodyB.getY() + overlap * (bodyA.getY() - bodyB.getY()) / distance);
			}
			
		}
		else if (bodyA.isFixed() || bodyB.isFixed())
		{
			if (bodyA.isFixed() && bodyB.isFixed())
			{
				bodyB.setX(bodyB.getX() + 2 * overlap * (bodyA.getX() - bodyB.getX()) / distance);
				bodyB.setY(bodyB.getY() + 2 * overlap * (bodyA.getY() - bodyB.getY()) / distance);
			}
			else
			{
				if (!bodyA.isFixed())
				{
					bodyA.setX(bodyA.getX() - 2 * overlap * (bodyA.getX() - bodyB.getX()) / distance);
					bodyA.setY(bodyA.getY() - 2 * overlap * (bodyA.getY() - bodyB.getY()) / distance);
				}
				else
				{
					bodyB.setX(bodyB.getX() + 2 * overlap * (bodyA.getX() - bodyB.getX()) / distance);
					bodyB.setY(bodyB.getY() + 2 * overlap * (bodyA.getY() - bodyB.getY()) / distance);
				}
			}
		}
		else
		{
			bodyA.setX(bodyA.getX() - overlap * (bodyA.getX() - bodyB.getX()) / distance);
			bodyA.setY(bodyA.getY() - overlap * (bodyA.getY() - bodyB.getY()) / distance);
		
			bodyB.setX(bodyB.getX() + overlap * (bodyA.getX() - bodyB.getX()) / distance);
			bodyB.setY(bodyB.getY() + overlap * (bodyA.getY() - bodyB.getY()) / distance);
		}
		
		
		distanceX = bodyA.getX() - bodyB.getX();
		distanceY = bodyA.getY() - bodyB.getY();
		distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
		
		float normalX = (bodyB.getX() - bodyA.getX()) / distance;
		float normalY = (bodyB.getY() - bodyA.getY()) / distance;

		float tangentX = -normalY;
		float tangentY = normalX;

		float dpTanA = bodyA.getspdX() * tangentX + bodyA.getspdY() * tangentY;
		float dpTanB = bodyB.getspdX() * tangentX + bodyB.getspdY() * tangentY;
		
		float dpNormA = bodyA.getspdX() * normalX + bodyA.getspdY() * normalY;
		float dpNormB = bodyB.getspdX() * normalX + bodyB.getspdY() * normalY;

		float mA = (dpNormA * (bodyA.getMass() - bodyB.getMass()) + 2.0f * bodyB.getMass() * dpNormB) / (bodyA.getMass() + bodyB.getMass());
		float mB = (dpNormB * (bodyB.getMass() - bodyA.getMass()) + 2.0f * bodyA.getMass() * dpNormA) / (bodyA.getMass() + bodyB.getMass());

		if (bodyA.isFixed() || bodyB.isFixed())
		{
			if (!bodyA.isFixed())
			{
				bodyA.setspdX(tangentX * dpTanA + normalX * mA);
				bodyA.setspdY(tangentY * dpTanA + normalY * mA);
			}
			else
			{
				bodyB.setspdX(tangentX * dpTanB + normalX * mB);
				bodyB.setspdY(tangentY * dpTanB + normalY * mB);
			}
		}
		else
		{
			bodyA.setspdX(tangentX * dpTanA + normalX * mA);
			bodyA.setspdY(tangentY * dpTanA + normalY * mA);
		
			bodyB.setspdX(tangentX * dpTanB + normalX * mB);
			bodyB.setspdY(tangentY * dpTanB + normalY * mB);
		}
	
	}

	void Gravity(Body& bodyA, Body& bodyB)
	{
		float dist = sqrt(pow(bodyA.getX() - bodyB.getX(), 2) + pow(bodyA.getY() - bodyB.getY(), 2));
		double attr = 9.81 * ((bodyA.getMass() * bodyB.getMass()) / pow(dist, 2));

		if (sBody != nullptr)
		{
			if (!sBody->Equals(bodyA))
			{
				bodyA.setspdX(bodyA.getspdX() + attr * ((bodyB.getX() - bodyA.getX()) / dist));
				bodyA.setspdY(bodyA.getspdY() + attr * ((bodyB.getY() - bodyA.getY()) / dist));
			}

			if (!sBody->Equals(bodyB))
			{
				bodyB.setspdX(bodyB.getspdX() - attr * ((bodyB.getX() - bodyA.getX()) / dist));
				bodyB.setspdY(bodyB.getspdY() - attr * ((bodyB.getX() - bodyA.getX()) / dist));
			}
		}
		else
		{
			bodyA.setspdX(bodyA.getspdX() + attr * ((bodyB.getX() - bodyA.getX()) / dist));
			bodyA.setspdY(bodyA.getspdY() + attr * ((bodyB.getY() - bodyA.getY()) / dist));
			
			bodyB.setspdX(bodyB.getspdX() - attr * ((bodyB.getX() - bodyA.getX()) / dist));
			bodyB.setspdY(bodyB.getspdY() - attr * ((bodyB.getX() - bodyA.getX()) / dist));
		}

		if (debugType > 1)
		{
			DrawLine(bodyA.getX(), bodyA.getY(), bodyA.getX() + 20 * attr * ((bodyB.getX() - bodyA.getX()) / dist), bodyA.getY() + 20 * attr * ((bodyB.getY() - bodyA.getY()) / dist), Color::Blue);
			DrawLine(bodyB.getX(), bodyB.getY(), bodyB.getX() - 20 * attr * ((bodyB.getX() - bodyA.getX()) / dist), bodyB.getY() - 20 * attr * ((bodyB.getY() - bodyA.getY()) / dist), Color::Blue);
		}
	}

	void Input(Event event)
	{
		if (event.type == Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left || event.mouseButton.button == Mouse::Right)
			{
				if (this->sBody != nullptr)
				{
					this->sBody = nullptr;
				}

				for (int x = 0; x < this->Corpses.size(); x++)
				{
					Vector2i mousePosition = Mouse::getPosition(this->renderer);
					mouseX = mousePosition.x;
					mouseY = mousePosition.y;

					if (PointBody(mousePosition.x, mousePosition.y, this->Corpses.at(x)))
					{
						sBody = &this->Corpses.at(x);
						sBody->setspdX(0);
						sBody->setspdY(0);

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
			if (sBody != nullptr)
			{
				mouseX = mousePosition.x;
				mouseY = mousePosition.y;

				if (sType == 1)
				{
					sBody->setX(mousePosition.x);
					sBody->setY(mousePosition.y);
				}
				
			}
		}
		
		if (event.type == Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == Mouse::Left || event.mouseButton.button == Mouse::Right)
			{
				if (sType == 2)
				{
					sBody->setspdX(0.1 * (sBody->getX() - mouseX));
					sBody->setspdY(0.1 * (sBody->getY() - mouseY));
				}

				sType = 0;
				if (this->sBody != nullptr)
				{
					this->sBody = nullptr;
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
		}
	}

	bool PointBody(int x, int y, Body body)
	{
		float dist = sqrt(pow(x - body.getX(), 2) + pow(y - body.getY(), 2));
		return (dist <= body.getSize());
	}

	void Move()
	{
		for (int i = 0; i < this->Corpses.size(); i++)
		{
			this->get(i)->Move();
		}
	}

	void Draw()
	{
		for (int i = 0; i < this->Corpses.size(); i++)
		{
			Body b = *this->get(i);
			DrawCircle(b.getX(),b.getY(),b.getSize(),b.getColor());
		}
	}

	void Debug()
	{
		
		this->frameTime = clock.restart();
		int framerate = 1 / (frameTime.asMilliseconds() * 0.001);
		DrawText(to_string(framerate), 0, 0, 30, Color::Yellow);
		
		DrawText("[D] Debug: " + to_string(debugType),this->renderer.getSize().x - 150, 0, 24, Color::Yellow);

		for (int i = 0; i < this->Corpses.size(); i++)
		{
			Body b = *this->get(i);
			
			if (debugType > 0)
			{
				DrawLine(b.getX(), b.getY(), b.getX() + 10 * b.getspdX(), b.getY() + 10 * b.getspdY(), Color::Red);
			}
		}

		for (int i = 0; i < Pairs.size(); i++)
		{
			Body a = *Pairs.at(i).first;
			Body b = *Pairs.at(i).second;
			
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

		if (sBody != nullptr)
		{
			if (sType == 2)
			{
				DrawLine(sBody->getX(), sBody->getY(), mouseX, mouseY, Color::Blue);
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

			this->Forces();
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
};