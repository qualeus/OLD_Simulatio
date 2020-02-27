#include "../Simulated/Bodies/Circle.cpp"

using namespace std;
using namespace sf;

class System
{
private:
	vector<Simulated*> Corpses;
	vector<pair<Body*, Body*>> Pairs;

	float force_X;
	float force_Y;
	bool gravity;


public:
	System(bool gravity = false, float force_X = 0.0f, float force_Y = 0.0f)
	{
		this->gravity = gravity;
		this->force_X = force_X;
		this->force_Y = force_Y;

		this->Corpses = vector<Simulated*>();
	}

	bool PairExist(Body* a, Body* b)
	{
		for (int i = 0; i < getPairsSize(); i++) {
			if ((a->Equals(Pairs.at(i).first) && b->Equals(Pairs.at(i).second)) || (a->Equals(Pairs.at(i).second) && b->Equals(Pairs.at(i).first)) ) { return true; }
		}
		return false;
	}

	void Prepare()
	{
		for (int x = 0; x < getCorpseSize(); x++)
		{
			Simulated* corpseA = getCorpse(x);

			if (corpseA->getClass() == 1)
			{
				for (int y = 0; y < getCorpseSize(); y++)
				{
					Simulated* corpseB = getCorpse(y);

					if (corpseB->getClass() == 1)
					{
						Body* bodyA = dynamic_cast<Body*>(corpseA);
						Body* bodyB = dynamic_cast<Body*>(corpseB);
						
						if (bodyA->Pairable(bodyB) && !PairExist(bodyA, bodyB)) { Pairs.push_back({bodyA, bodyB}); }
					}
				}
				
			}
		}
	}

	void Forces()
	{
		for (int i = 0; i < getPairsSize(); i++)
		{
			Body* corpseA = getPairA(i);
			Body* corpseB = getPairB(i);

			switch (corpseB->getClass())
			{
				case id_Body:
				{

				} break;

				case id_Circle:
				{
					if (corpseA->Collide(corpseB)) 
					{ 
						Circle* circleB = dynamic_cast<Circle*>(corpseB);
						corpseA->CollisionCircle(circleB); 
					}
				} break;
			}
			
			if (this->gravity) { corpseA->Gravity(corpseB); }	
			
		}

		for (int i = 0; i < getCorpseSize(); i++)
		{
			Simulated* corpse = getCorpse(i);
			corpse->Launch(this->getForce_X(), this->getForce_Y());
		}
	}

	void CorpsesStep() { for (int i = 0; i < getCorpseSize(); i++) { getCorpse(i)->Step(); } }

	void Step()
	{
		Forces();
		CorpsesStep();
	}

	float getForce_X() {return this->force_X; }
	void setForce_X(float Force_X) { this->force_X = Force_X; }

	float getForce_Y() {return this->force_Y; }
	void setForce_Y(float Force_Y) { this->force_Y = Force_Y; }

	bool getGravity() { return this->gravity; }
	void setGravity(bool gravity) { this->gravity = gravity; } 

	vector<Simulated*> getCorpses() { return this->Corpses; }
	vector<pair<Body*, Body*>> getPairs() { return this->Pairs; }

	void addCorpse(Simulated* corpse) { this->Corpses.emplace_back(corpse); }
	void addCorpse(Body* corpse) { this->Corpses.emplace_back(corpse); }
	void addCorpse(Circle* corpse) { this->Corpses.emplace_back(corpse); }
		
	int getCorpseSize() { return this->Corpses.size(); }
	Simulated* getCorpse(int index) {

		if (index >= 0 && index < getCorpseSize()) 
		{
			cout << "Simu: " << this->Corpses.at(index)->getClass() << endl;
			switch (this->Corpses.at(index)->getClass())
			{
				case id_Body:
				{
					Body* body = dynamic_cast<Body*>(this->Corpses.at(index));
					return body;
				} break;

				case id_Circle:
				{
					Circle* circle = dynamic_cast<Circle*>(this->Corpses.at(index));
					
					cout << circle->getClass() << endl;
					return circle;
				} break;

				default:
				{
					return this->Corpses.at(index);
				}
			}
		} 
		else 
		{ 
			return nullptr; 
		} 
	}
	
	int getPairsSize() { return this->Pairs.size(); }
	Body* getPairA(int index){ if (index >= 0 && index < getPairsSize()) { return this->Pairs.at(index).first; } else { return nullptr; } }
	Body* getPairB(int index){ if (index >= 0 && index < getPairsSize()) { return this->Pairs.at(index).second; } else { return nullptr; } }
};