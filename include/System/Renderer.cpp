#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <math.h>

#include "../System/System.cpp"

using namespace std;
using namespace sf;

class Renderer
{
private:
	System system;

	RenderWindow renderer;
	View view;
	Clock clock;
	Time frame;

	Simulated* Simulated_cursor;
	Simulated Simulated_save;

	int select_Type;
	int debug_Type;
	
	float mouse_X;
	float mouse_Y;

	bool camera_P;
	float camera_Zoom;
	float camera_X;
	float camera_Y;

	string name;

public:
	Renderer(bool gravity = false, float force_X = 0.0f, float force_Y = 0.0f, int screen_X = 1200, int screen_Y = 800, string name = "Default system")
	{
		//SYSTEM
		this->system = System(gravity, force_X, force_Y);
		this->name = name;

		//DEBUG
		this->select_Type = 0;
		this->debug_Type = 0;
		this->camera_P = false;

		this->Simulated_cursor = nullptr;
		this->Simulated_save = Circle();

		//CAMERA
		this->camera_Zoom = 100.0f;
		this->camera_X = 0.0f;
		this->camera_Y = 0.0f;

		this->view.reset(FloatRect(this->camera_X, this->camera_Y, this->camera_X + screen_X, this->camera_Y + screen_Y));	

		//RENDERER
		ContextSettings settings;
		settings.antialiasingLevel = 8;
		
		this->renderer.create(VideoMode(screen_X, screen_Y), this->name, Style::Default, settings);
		this->renderer.setView(this->view);
		this->renderer.setFramerateLimit(60);	
		
	}
	
	void Render()
	{		

		this->system.Prepare();

		while (this->renderer.isOpen())
		{
			//SYSTEM
			if (!Paused()) { this->system.Step(); }

			//RENDERER
			this->renderer.clear(sf::Color::Black);

			Event event;
			while (this->renderer.pollEvent(event)) { Input(event, getCorpses()); }

			this->Draw();

			//DEBUG
			this->frame = clock.restart();
			this->Debug();

			this->renderer.display();
		}
	}

	void Close() { this->renderer.close(); }

	void Selector(Simulated* corpse, float x, float y, int select_type = 1)
	{
		switch (corpse->getClass())
		{
			case id_Body:
			{
				Body* body = dynamic_cast<Body*>(corpse);
				if (body->Pointed(x, y))
				{
					this->Simulated_save = *body;

					body->setFixed(true);
					this->Simulated_cursor = body;
					this->select_Type = select_type;
				}
			} break;

			case id_Circle:
			{
				Circle* circle = dynamic_cast<Circle*>(corpse);
				if (circle->Pointed(x, y))
				{
					this->Simulated_save = *circle;

					circle->setFixed(true);
					this->Simulated_cursor = circle;
					this->select_Type = select_type;
				}
			} break;
		}
	}

	void MoveCorpse(Simulated* corpse, float x, float y)
	{
		if (corpse != nullptr && this->select_Type == 1)
		{
			switch (corpse->getClass())
			{
				case id_Body:
				{
					this->Simulated_cursor->Move(x, y);
				} break;

				case id_Circle:
				{
					this->Simulated_cursor->Move(x, y);
				} break;
			}
		}
	}

	void LaunchCorpse(Simulated* corpse, float x, float y)
	{
		if (corpse != nullptr && select_Type == 2)
		{
			switch (corpse->getClass())
			{
				case id_Body:
				{
					Body* body = dynamic_cast<Body*>(corpse);
					float x_speed = 0.1 * body->getX() - x;
					float y_speed = 0.1 * body->getY() - y;

					getSimulated_cursor()->Launch(x_speed, y_speed);
				} break;

				case id_Circle:
				{
					Circle* circle = dynamic_cast<Circle*>(corpse);
					float x_speed = 0.1 * circle->getX() - x;
					float y_speed = 0.1 * circle->getY() - y;

					getSimulated_cursor()->Launch(x_speed, y_speed);
				} break;
			}
		}
	}

	void ResetSelected()
	{
		if (getSimulated_cursor() != nullptr)
		{
			getSimulated_cursor()->setFixed(getSimulated_save().getFixed());
			setSimulated_cursor(nullptr);
		}

		this->select_Type = 0;
	}

	void NextDebugType() { if (this->debug_Type < 3) { this->debug_Type += 1; } else { this->debug_Type = 0; } }
	void TogglePause() { this->camera_P = !this->camera_P; }

	void Input(Event event, vector<Simulated*> Corpses)
	{
		Vector2i mousePosition = Mouse::getPosition(this->renderer);
		this->mouse_X = mousePosition.x + this->camera_X;
		this->mouse_Y = mousePosition.y + this->camera_Y;

		if (event.type == Event::Closed)
		{
			Close();
		}
		else if (event.type == Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
				case Mouse::Left:
					for (int i = 0; i < getCorpsesSize(); i++) { Selector(getCorpse(i), this->mouse_X, this->mouse_Y, 1); }
					break;

				case Mouse::Right:
					for (int i = 0; i < getCorpsesSize(); i++) { Selector(getCorpse(i), this->mouse_X, this->mouse_Y, 2); }
					break;
			}
		}
		else if (event.type == Event::MouseMoved)
		{
			MoveCorpse(getSimulated_cursor(), this->mouse_X, this->mouse_Y);
		}
		else if (event.type == Event::MouseButtonReleased)
		{
			switch (event.mouseButton.button)
			{
				case Mouse::Left:
					LaunchCorpse(getSimulated_cursor(), this->mouse_X, this->mouse_Y);
					ResetSelected();
					break;

				case Mouse::Right:
					LaunchCorpse(getSimulated_cursor(), this->mouse_X, this->mouse_Y);
					ResetSelected();
					break;
			}
		}

		if (event.type == Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case Keyboard::D:
					NextDebugType();
					break;
				case Keyboard::Space:
					TogglePause();
					break;
			}
		}
	}

	int Framerate() { return (1 / (this->frame.asMilliseconds() * 0.001)); }

	void DrawInterface()
	{
		DrawText(to_string(Framerate()), camera_X + 0, camera_Y + 0, 30, Color::Yellow);
		DrawText("camera_X: " + to_string(this->camera_X), camera_X + 0, camera_Y + 30, 20, Color::Yellow);
		DrawText("camera_Y: " + to_string(this->camera_Y), camera_X + 0, camera_Y + 50, 20, Color::Yellow);
		DrawText("camera_Zoom: " + to_string(this->camera_Zoom), camera_X + 0, camera_Y + 70, 20, Color::Yellow);
		DrawText("paused: " + to_string(this->camera_P), camera_X + 0, camera_Y + 90, 20, Color::Yellow);
		DrawText("[D] Debug: " + to_string(debug_Type),camera_X + this->renderer.getSize().x - 150, camera_Y + 0, 24, Color::Yellow);
	}


	void Draw()
	{
		for (int i = 0; i < getCorpsesSize(); i++)
		{
			Simulated* corpse = getCorpse(i);
			
			switch (corpse->getClass())
			{
				case id_Body:
				{
					Body* body = dynamic_cast<Body*>(corpse);
					
				} break;

				case id_Circle:
				{
					Circle* circle = dynamic_cast<Circle*>(corpse);
					DrawCircle(circle->getX(), circle->getY(), circle->getSize(), circle->getColor());
				} break;

			}
			
		}
	}

	void DrawDebugSpeed(Simulated* corpse)
	{
		switch (corpse->getClass())
		{
			case id_Body:
			{
				Body* body = dynamic_cast<Body*>(corpse);
				DrawLine(body->getX(), body->getY(), body->getX() + 10 * body->getX_speed(), body->getY() + 10 * body->getY_speed(), Color::Red);

			} break;

			case id_Circle:
			{
				Circle* circle = dynamic_cast<Circle*>(corpse);
				DrawLine(circle->getX(), circle->getY(), circle->getX() + 10 * circle->getX_speed(), circle->getY() + 10 * circle->getY_speed(), Color::Red);

			} break;
		}
	}

	void DrawDebugPairs(Simulated* corpseA, Simulated* corpseB)
	{
		switch (corpseA->getClass())
		{
			case id_Body:
			{
				Body* body = dynamic_cast<Body*>(corpseA);
				DrawLine(body->getX(), body->getY(), body->getX() + 10 * body->getX_speed(), body->getY() + 10 * body->getY_speed(), Color::Red);

			} break;

			case id_Circle:
			{
				Circle* circle = dynamic_cast<Circle*>(corpseA);
				DrawLine(circle->getX(), circle->getY(), circle->getX() + 10 * circle->getX_speed(), circle->getY() + 10 * circle->getY_speed(), Color::Red);

			} break;
		}
	}

	void DrawDebugDrag() 
	{ 
		if (select_Type == 2 && Simulated_cursor != nullptr)
		{
			switch (Simulated_cursor->getClass())
			{
				case id_Body:
				{
					Body* Body_cursor = dynamic_cast<Body*>(Simulated_cursor);
					DrawLine(Body_cursor->getX(), Body_cursor->getY(), this->mouse_X, this->mouse_Y, Color::Blue);
				} break;

				case id_Circle:
				{
					Circle* Body_cursor = dynamic_cast<Circle*>(Simulated_cursor);
					DrawLine(Body_cursor->getX(), Body_cursor->getY(), this->mouse_X, this->mouse_Y, Color::Blue);
				} break;
			}
		}
	}
	
	void Debug()
	{
		DrawInterface();
		
		DrawDebugDrag();

		switch (this->debug_Type)
		{
			case 1:
			{
				for (int i = 0; i < getCorpsesSize(); i++) { DrawDebugSpeed(getCorpse(i)); }

			} break;

			case 2:
			{
				for (int i = 0; i < getPairsSize(); i++) { DrawDebugPairs(getPairA(i), getPairB(i)); }
			
			} break;
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
	
	void Camera(float x = 0.0f, float y = 0.0f, float zoom = 1.0f)
	{
		this->camera_X = this->camera_X + x;
		this->camera_Y = this->camera_Y + y;
		this->camera_Zoom = this->camera_Zoom * zoom;

		this->view.move(x, y);
		this->view.zoom(zoom);
		this->renderer.setView(this->view);
	}

	vector<Simulated*> getCorpses(){ return this->system.getCorpses(); }
	int getCorpsesSize(){ return this->system.getCorpseSize(); }
	Simulated* getCorpse(int i) { return this->system.getCorpse(i); }

	vector<pair<Body*, Body*>> getPairs(){ return this->system.getPairs(); }
	int getPairsSize() { return this->system.getPairsSize(); }
	Body* getPairA(int index){ return this->system.getPairA(index); }
	Body* getPairB(int index){ return this->system.getPairB(index); }
	

	Simulated* getSimulated_cursor() { return this->Simulated_cursor; }
	void setSimulated_cursor(Simulated* Simulated_cursor) { this->Simulated_cursor = Simulated_cursor; }

	Simulated getSimulated_save() { return this->Simulated_save; }
	void setSimulated_save(Simulated Simulated_save) { this->Simulated_save = Simulated_save; }

	void addCorpse(Simulated corpse) { this->system.addCorpse(&corpse); }
	void addCorpse(Body corpse) { this->system.addCorpse(&corpse); }
	void addCorpse(Circle corpse) {	this->system.addCorpse(&corpse); }

	bool Paused() { return this->camera_P; }	
	
};