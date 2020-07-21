#include "../include/Polygon.hpp"

namespace phy {

Polygon::Polygon(float x, float y, float size, float mass, float damping, float speed_x, float speed_y, bool fixed, bool etherial, sf::Color color):Corpse(x, y, mass, damping, fixed, etherial) {
}

Polygon::~Polygon() {}

const int Polygon::get_class() { return ID_POLYGON;}

void Polygon::Move(float x, float y) { this->current_pos = this->current_pos+sf::Vector2f(x, y); }
void Polygon::Move(sf::Vector2f move) { this->current_pos = this->current_pos+move; }
bool Polygon::inBounds(float x1, float x2, float y1, float y2) { return true; }
bool Polygon::Pointed(float x, float y) { return false; }

void Polygon::Collision(std::shared_ptr<Corpse> a) {
	
}

float Polygon::get_size() { return this->size; }
vtr::Rectangle Polygon::get_corpse_bounds() { return vtr::Rectangle({sf::Vector2f(), sf::Vector2f()}); }

}
