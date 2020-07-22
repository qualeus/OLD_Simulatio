#include "../include/Corpse.hpp"

namespace phy {

Corpse::Corpse(float x, float y, float mass, float damping, bool fixed, bool etherial) {
	static int global_corpse_id = 0;
	this->id = global_corpse_id++;

	this->fixed = fixed;
	this->etherial = etherial;
	this->removed = false;
	this->current_pos = sf::Vector2f(x, y);
	this->mass = mass;
	set_damping(damping);
}

Corpse::~Corpse() = default;

int Corpse::get_id() const { return this->id; }
int Corpse::get_class() const { return ID_CORPSE;}

bool Corpse::get_fixed() const { return this->fixed; }
void Corpse::set_fixed(bool fixed) { this->fixed = fixed; }

bool Corpse::get_etherial() const { return this->etherial; }
void Corpse::set_etherial(bool etherial) { this->etherial = etherial; }

bool Corpse::get_removed() const { return this->removed; }
void Corpse::Remove() { this->removed = true; }

void Corpse::Step() {}
void Corpse::Stop() {}
void Corpse::Move(float x, float y) {}
void Corpse::Move(sf::Vector2f move) {}
void Corpse::Collision(std::shared_ptr<Corpse> a) {}

sf::Color Corpse::get_color() { return this->color; }
void Corpse::set_color(sf::Color color) { this->color = color; }

sf::Vector2f Corpse::get_pos() { return this->current_pos; }
float Corpse::get_pos_x() { return this->current_pos.x; }
float Corpse::get_pos_y() { return this->current_pos.y; }

void Corpse::set_pos(sf::Vector2f pos) { this->current_pos = pos; }
void Corpse::set_pos_x(float pos_x) { this->current_pos.x = pos_x; }
void Corpse::set_pos_y(float pos_y) { this->current_pos.y = pos_y; }

sf::Vector2f Corpse::get_last_pos() { return this->last_pos; }
float Corpse::get_last_pos_x() { return this->last_pos.x; }
float Corpse::get_last_pos_y() { return this->last_pos.y; }

void Corpse::set_last_pos(sf::Vector2f pos) { this->last_pos = pos; }
void Corpse::set_last_pos_x(float pos_x) { this->last_pos.x = pos_x; }
void Corpse::set_last_pos_y(float pos_y) { this->last_pos.y = pos_y; }

void Corpse::set_damping(float damping) { if (damping < 0.1f) { this->damping = 0.1f; } else { this->damping = damping; } }
float Corpse::get_bounce() { return 1.0f/this->damping; }
float Corpse::get_mass() { return this->mass; }

bool Corpse::Equals(const Corpse* other) { 	
	if (this->get_id() != other->get_id()) { return false; }
	if (this->get_class() != other->get_class()) { return false; }
	if (this->get_fixed() != other->get_fixed()) { return false; }
	if (this->get_etherial() != other->get_etherial()) { return false; }
	return true;
}

inline bool Corpse::operator==(const Corpse* other) {
	return this->Equals(other);
}

}