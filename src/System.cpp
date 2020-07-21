#include "../include/System.hpp"

namespace phy {

System::System(bool gravity, float force_x, float force_y) {
	this->gravity = gravity;
	this->force_x = force_x;
	this->force_y = force_y;
	this->dt = 1000;

	this->corpses = std::vector<std::shared_ptr<Corpse>>();
	this->pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
	this->quad_pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();

	this->quadtree = Quadtree({sf::Vector2f(-50.0f, -250.0f), sf::Vector2f(1000.0f, 1000.0f)}, 1);

	this->corpses_size = 0;
	this->pairs_size = 0;
}

System::~System() {}

void System::Prepare() {
	// temp one time but to update every frame
	this->quadtree.clear();
	for (int i = 0; i < corpses_size; i++) { this->quadtree.insert(get_corpse(i)); }
}

void System::Step() {
	Prepare();
	CorpsesStep();
	PairsStep();
}

void System::CorpsesStep() {
	for (int i = 0; i < corpses_size; i++) {
		get_corpse(i)->Step(); 
		if (!get_corpse(i)->get_fixed()) { get_corpse(i)->Move(sf::Vector2f(this->force_x, this->force_y)); }
	}
}

void  System::CorpseStop(int i) {
	std::shared_ptr<Corpse> corpse = get_corpse(i);

	if (phy::Circle* circle = dynamic_cast<phy::Circle*>(corpse.get())) {
		circle->Stop();
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(corpse.get())) {
    	
    }
	
}

void System::PairsStep() {
	for (int i = 0; i < pairs_size; i++) { 
		if (this->gravity) { Forces(get_pair_A(i), get_pair_B(i)); }
		//Collision(get_pair_A(i), get_pair_B(i));
	}

	std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> quadpairs = this->quadtree.make_pairs();
	this->quad_pairs = quadpairs;
	for (int i = 0; i < quadpairs.size(); i++) { Collision(quadpairs.at(i).first, quadpairs.at(i).second); }
}

void System::Collision(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {

    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(a.get())) {
		circle->Collision(b);
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(a.get())) {
    	polygon->Collision(b);
    }
}

void System::Forces(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {

	// Gravity
	float size = a->get_size() + b->get_size();
	float dist = vtr::Length(a->get_pos_x(), a->get_pos_y(), b->get_pos_x(), b->get_pos_y()) + 10;
	
	// G * (ma * mb)/(r^2)
	float force = G * ((a->get_mass() * b->get_mass()) / pow(dist, 2));

	if (force > LS) { force = LS; }

	float normal_mass_a;
	float normal_mass_b;

	if (a->get_fixed() && b->get_fixed()) {
		normal_mass_a = 0;
		normal_mass_b = 0;
	} else if (a->get_fixed()) {
		normal_mass_a = 0;
		normal_mass_b = 1;
	} else if (b->get_fixed()) {
		normal_mass_a = 1;
		normal_mass_b = 0;
	} else {
		float normal_mass = a->get_mass() + b->get_mass();
		normal_mass_a = a->get_mass() / normal_mass;
		normal_mass_b = b->get_mass() / normal_mass;
	}

	a->Move(sf::Vector2f((b->get_pos_x() - a->get_pos_x()) / dist, (b->get_pos_y() - a->get_pos_y()) / dist) * force * normal_mass_a);
	b->Move(sf::Vector2f((a->get_pos_x() - b->get_pos_x()) / dist, (a->get_pos_y() - b->get_pos_y()) / dist) * force * normal_mass_b);
}

std::shared_ptr<Quadtree> System::get_quadtree() { return std::make_shared<Quadtree>(this->quadtree); }

int System::get_dt() { return this->dt; }
void System::set_dt(int dt) { this->dt = dt; }
void System::add_dt(int dt) { this->dt += dt; }

float System::get_force_x() { return this->force_x; }
void System::set_force_x(float force_x) { this->force_x = force_x; }

float System::get_force_y() { return this->force_y; }
void System::set_force_y(float force_y) { this->force_y = force_y; }

bool System::get_gravity() { return this->gravity; }
void System::set_gravity(bool gravity) { this->gravity = gravity; }

int System::get_corpses_size() { return this->corpses_size; }
int System::get_pairs_size() { return this->pairs_size; }
int System::get_quad_pairs_size() { return this->quad_pairs.size(); }

void System::addCorpse(Polygon a) { add_corpse(std::make_shared<Polygon>(a)); }
void System::addCorpse(Circle a) { add_corpse(std::make_shared<Circle>(a)); }

void System::add_corpse(std::shared_ptr<Corpse> a) {
	this->corpses.emplace_back(std::move(a));
	this->corpses_size++; // Update the size of the array = n

	if (corpses_size > 1) {
		for (int b = 0; b < corpses_size - 1; b++) { System::add_pair(a, get_corpse(b)); }
	}
}

void  System::add_corpse(std::shared_ptr<Circle> a) {
	this->corpses.emplace_back(std::move(a));
	this->corpses_size++; // Update the size of the array = n

	if (corpses_size > 1) {
		for (int b = 0; b < corpses_size - 1; b++) { System::add_pair(get_corpse(corpses_size-1), get_corpse(b)); }
	}
}

void System::add_pair(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b) {
	this->pairs.push_back({a, b});

	this->pairs_size++; // Update the size of the array = [n(n-1)]/2
}

std::vector<std::shared_ptr<Corpse>> System::get_corpses() { return this->corpses; }
std::shared_ptr<Corpse> System::get_corpse(int index) { if (index >= 0 && index < get_corpses_size()) { return this->corpses.at(index); } else { return nullptr; } }

std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>  System::get_pairs() { return this->pairs; }
std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>  System::get_pair(int index) { if (index >= 0 && index < get_pairs_size()) { return this->pairs.at(index); } else { return {nullptr, nullptr}; } }
std::shared_ptr<Corpse> System::get_pair_A(int index) { if (index >= 0 && index < get_pairs_size()) { return this->pairs.at(index).first; } else { return nullptr; } }
std::shared_ptr<Corpse>  System::get_pair_B(int index) { if (index >= 0 && index < get_pairs_size()) { return this->pairs.at(index).second; } else { return nullptr; } }

std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> System::get_quad_pair(int index) { if (index >= 0 && index < this->quad_pairs.size()) { return this->quad_pairs.at(index); } else { return {nullptr, nullptr}; } }

}