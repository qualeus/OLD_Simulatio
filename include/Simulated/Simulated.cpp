#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

using namespace std;
using namespace sf;

const int id_Simulated = 0;
const int id_Body = 1;
const int id_Circle = 10;

class Simulated
{
protected:
	int id;
	bool fixed;
	bool etherial;

public:

	Simulated(bool fixed = false, bool etherial = false)
	{
		static int global_id = 0;
		this->id = global_id++;
	}
	
	virtual ~Simulated() {};

	virtual int getClass(){ return id_Simulated; };

	bool Equals(Simulated* simulated) { return (this->id == simulated->id); }

	virtual void Step(){ };
	bool Pointed(float x, float y){ return false; };

	bool Pairable(Simulated* corpse){ return false; };

	void Move(float x, float y){ };
	void Launch(float x_speed, float y_speed){ };

	bool getFixed() { return this->fixed; }
	void setFixed(bool fixed) { this->fixed = fixed; }

	bool getEtherial() { return this->etherial; }
	void setEtherial(bool etherial) { this->etherial = etherial; }
};