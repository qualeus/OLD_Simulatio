#include "../include/Circle.hpp"

namespace phy {

Circle::Circle(float x, float y, float size, float mass, float damping, float speed_x, float speed_y, bool fixed, bool etherial, sf::Color color):Corpse(x, y, mass, damping, fixed, etherial) {
	this->last_pos = sf::Vector2f(x-speed_x, y-speed_y);
	this->size = size;
	this->color = color;
}

Circle::~Circle() {}

const int Circle::get_class() { return ID_CIRCLE;}

void Circle::Step() {
	if (this->fixed)  {
		this->last_pos = this->current_pos;
	} else {
		sf::Vector2f diff_pos = this->current_pos - this->last_pos;
		this->last_pos = this->current_pos;
		this->current_pos = this->current_pos + diff_pos;
	}
}
void Circle::Stop() { this->last_pos = this->current_pos; }

void Circle::Move(float x, float y) { this->current_pos = this->current_pos+sf::Vector2f(x, y); }
void Circle::Move(sf::Vector2f move) { this->current_pos = this->current_pos+move; }
bool Circle::inBounds(float x1, float x2, float y1, float y2) {
	return ((this->current_pos.x + this->size > x1) && (this->current_pos.x - this->size < x2) && (this->current_pos.y + this->size > y1) && (this->current_pos.y - this->size < y2)) || ((this->current_pos.x > x1) && (this->current_pos.x < x2) && (this->current_pos.y > y1) && (this->current_pos.y < y2)) ;
}

bool Circle::Pointed(float x, float y) {
	return (vtr::Length(this->get_pos_x(), this->get_pos_y(), x, y) <= this->size);
}

void Circle::Collision(std::shared_ptr<Corpse> a) {
	if (Circle* circle = dynamic_cast<Circle*>(a.get())) {
		// Circle on Circle Collision
		float distance = vtr::Length(this->get_pos_x(), this->get_pos_y(), circle->get_pos_x(), circle->get_pos_y());
		bool asymetric = this->get_fixed() || circle->get_fixed();		
		
		// Repulsion for avoid the corpses superposition
		float overlap = (this->get_size() + circle->get_size() - distance) * 0.5f;
		float x_diff = this->get_pos_x() - circle->get_pos_x();
		float y_diff = this->get_pos_y() - circle->get_pos_y();

		float damping = (this->get_bounce() + circle->get_bounce()) * 0.5f;

		if (overlap > 0) {
			if (asymetric) {
				if (!this->get_fixed()) {
					this->Move(sf::Vector2f(x_diff / distance, y_diff / distance) * overlap * damping * 2.0f);
				} else if (!circle->get_fixed()) {
					circle->Move(sf::Vector2f(x_diff / distance, y_diff / distance) * -overlap * damping * 2.0f);
				} else {
					this->Move(sf::Vector2f(x_diff / distance, y_diff / distance) * overlap);
					circle->Move(sf::Vector2f(x_diff / distance, y_diff / distance) * -overlap);
				}
			} else {
				float normal_mass = this->get_mass() + circle->get_mass();
				float normal_mass_this = this->get_mass() / normal_mass;
				float normal_mass_circle = circle->get_mass() / normal_mass;

				this->Move(sf::Vector2f(x_diff / distance, y_diff / distance) * overlap * damping * normal_mass_circle);
				circle->Move(sf::Vector2f(x_diff / distance, y_diff / distance) * -overlap * damping * normal_mass_this);
			}
		}
		
		
    } else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {
    	// Circle / Polygon
    	
    }
}

float Circle::get_size() { return this->size; }

vtr::Rectangle Circle::get_corpse_bounds() { return vtr::Rectangle({sf::Vector2f(this->get_pos_x()-this->get_size(), this->get_pos_y()-this->get_size()), sf::Vector2f(this->get_size() * 2.0f, this->get_size() * 2.0f)}); }

}