#include "../include/Circle.hpp"

namespace phy {

Circle::Circle(float x, float y, float size, float mass, float damping, float speed_x, float speed_y, bool fixed, bool etherial, sf::Color color):Corpse(x, y, mass, damping, fixed, etherial, color) {
	this->last_pos = sf::Vector2f(x-speed_x, y-speed_y);
	this->size = size;
}

Circle::~Circle() {}

int Circle::get_class() const { return ID_CIRCLE;}

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

void Circle::Move(float x, float y, bool relative) { 
	if(relative) {
		this->current_pos = this->current_pos+sf::Vector2f(x, y); 
	} else {
		this->current_pos = sf::Vector2f(x, y); 
	}
}
void Circle::Move(sf::Vector2f move, bool relative) { 
	if (relative) {
		this->current_pos = this->current_pos+move;
	} else {
		this->current_pos = move;
	} 
}
bool Circle::inBounds(float x1, float x2, float y1, float y2) {
	return ((this->current_pos.x + this->size > x1) && (this->current_pos.x - this->size < x2) && (this->current_pos.y + this->size > y1) && (this->current_pos.y - this->size < y2)) || ((this->current_pos.x > x1) && (this->current_pos.x < x2) && (this->current_pos.y > y1) && (this->current_pos.y < y2)) ;
}

bool Circle::Pointed(float x, float y) {
	return (ftn::Length(this->get_pos_x(), this->get_pos_y(), x, y) <= this->size);
}

void Circle::Collision(std::shared_ptr<Corpse> a) {
	if (Circle* circle = dynamic_cast<Circle*>(a.get())) {

		// Circle / Circle Collision
		float distance = ftn::Length(this->get_pos(), circle->get_pos());
		float overlap = (this->get_size() + circle->get_size() - distance);
		if (overlap < 0) { return; }
			
		float x_diff = this->get_pos_x() - circle->get_pos_x();
		float y_diff = this->get_pos_y() - circle->get_pos_y();

		sf::Vector2f vector_response = sf::Vector2f(x_diff / distance, y_diff / distance) * overlap;

		Corpse::CollisionResponse(this, circle, vector_response);
    } else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {
    	
    	// Circle / Polygon collision
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = polygon->get_sides();

		for (int i=0; i<sides.size(); i++) {
			auto test_intersect = ftn::Line_Circle_Intersect(sides.at(i).first, sides.at(i).second, this->get_pos(), this->get_size());
			
			// Collide if one side of the polygon intersect with the circle 
			if (test_intersect.first) {
				sf::Vector2f vector_response = ftn::Normalize(this->get_pos() - test_intersect.second) * (ftn::Length(this->get_pos(), test_intersect.second)-this->get_size());
				Corpse::CollisionResponse(polygon, this, vector_response);
				return;
			}
		}

		// Collide if the center of the circle is in the polygon
		if (polygon->Pointed(this->get_pos().x, this->get_pos().y)) {
			sf::Vector2f vector_response = polygon->get_pos() - this->get_pos();
			Corpse::CollisionResponse(polygon, this, vector_response);
			return;	
		}
    }
}

float Circle::get_size() const { return this->size; }

ftn::Rectangle Circle::get_corpse_bounds() const { return ftn::Rectangle({sf::Vector2f(this->get_pos_x()-this->get_size(), this->get_pos_y()-this->get_size()), sf::Vector2f(this->get_size() * 2.0f, this->get_size() * 2.0f)}); }

}