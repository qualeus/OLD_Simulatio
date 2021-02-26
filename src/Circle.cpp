#include "../include/Circle.hpp"

namespace phy {

Circle::Circle(float x, float y, float size, float mass, float damping, float speed_x, float speed_y, float rotation, float motor, bool fixed, bool tied, bool etherial, sf::Color color):Corpse(mass, damping, fixed, tied, etherial, color) {
	this->set_pos(sf::Vector2f(x, y));
	this->set_last_pos(sf::Vector2f(x-speed_x, y-speed_y));

	this->size = size;
	this->last_rotation = rotation;
	this->motor_rotation = motor;
}

Circle::~Circle() {}

int Circle::get_class() const { return ID_CIRCLE;}

void Circle::Step() {
	if (this->fixed)  {
		this->set_last_pos(this->get_pos());
	} else {
		sf::Vector2f diff_pos = this->get_velocity();
		this->set_last_pos(this->get_pos());
		this->set_pos(this->get_pos() + diff_pos);
	}

	if (this->tied) {
		this->set_last_rotation(this->get_rotation());
	} else {
		float diff_rotation = this->get_rotation_speed();
		this->set_last_rotation(this->get_rotation());
		this->set_rotation(this->get_rotation() + diff_rotation);
	}
	
	if (!ftn::decimal_equals(motor_rotation, 0.0f, 0.0001f)) { this->set_rotation(this->get_rotation() + motor_rotation); }
}

void Circle::Stop() { 
	this->set_last_pos(this->get_pos());
	this->set_last_rotation(this->get_rotation());
}

void Circle::Move(float x, float y, bool relative) { 
	if(relative) {
		this->set_pos(this->get_pos() + sf::Vector2f(x, y));
	} else {
		this->set_pos(sf::Vector2f(x, y)); 
	}
}

void Circle::Move(sf::Vector2f move, bool relative) { 
	if (relative) {
		this->set_pos(this->get_pos() + move);
	} else {
		this->set_pos(move);
		this->Stop();
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
		float optimized_radius = this->get_size() + circle->get_size();
		optimized_radius *= optimized_radius; /* Faster to square both parts insteads of using square root */
		float optimized_overlap = ftn::Squared_Length(this->get_pos(), circle->get_pos());

		if (optimized_overlap > optimized_radius) { return; }

		float distance = ftn::Length(this->get_pos(), circle->get_pos());
		float overlap = (this->get_size() + circle->get_size() - distance);

		float x_diff = this->get_pos_x() - circle->get_pos_x();
		float y_diff = this->get_pos_y() - circle->get_pos_y();

		sf::Vector2f vector_response = sf::Vector2f(x_diff / distance, y_diff / distance) * overlap;
		Corpse::CollisionResponse(this, circle, vector_response);
    } else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {
    	
    	// Circle / Polygon collision

		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = polygon->get_sides();

		// Collide if the center of the circle is in the polygon
		if (polygon->Pointed(this->get_pos().x, this->get_pos().y)) {
			
			// Find the closest point on edges
			std::pair<sf::Vector2f, sf::Vector2f> closest_side = ftn::Closest_Edge(sides, this->get_pos());
			
			sf::Vector2f closest_projection = ftn::Segment_Projection(closest_side.first, closest_side.second, this->get_pos());
			sf::Vector2f vector_response = ftn::Normalize(ftn::Norme(closest_side.second, closest_side.first))*(ftn::Length(this->get_pos(), closest_projection)+this->get_size());
			Corpse::CollisionResponse(polygon, this, vector_response);
			return;	
		}

    	// Collide if one side of the polygon intersect with the circle 
    	
		for (int i=0; i<sides.size(); i++) {
			auto test_intersect = ftn::Line_Circle_Intersect(sides.at(i).first, sides.at(i).second, this->get_pos(), this->get_size());
			
			// Don't collide with any edge
			if (test_intersect.first == 0) { continue; }
			
			if (test_intersect.first == 1) {

				// Collide at the middle of an edge
				sf::Vector2f vector_response = ftn::Normalize(this->get_pos() - test_intersect.second) * (ftn::Length(this->get_pos(), test_intersect.second)-this->get_size());
				Corpse::CollisionResponse(polygon, this, vector_response);
				return;

			} else if (test_intersect.first == 2) {

				// Collide with the first point of the edge (current edge + last edge)
				int last_edge = (i-1) % sides.size();
				sf::Vector2f normals_average = ftn::Norme(sides.at(last_edge).first, sides.at(last_edge).second) + ftn::Norme(sides.at(i).first, sides.at(i).second);
				sf::Vector2f vector_response = ftn::Normalize(normals_average) * (ftn::Length(this->get_pos(), sides.at(i).first)-this->get_size());
				Corpse::CollisionResponse(polygon, this, vector_response);
				return;

			} else if (test_intersect.first == 3) {

				// Collide with the second point of the edge (current edge + next edge)
				int next_edge = (i+1) % sides.size();
				sf::Vector2f normals_average = ftn::Norme(sides.at(i).first, sides.at(i).second) + ftn::Norme(sides.at(next_edge).first, sides.at(next_edge).second);
				sf::Vector2f vector_response = ftn::Normalize(normals_average) * (ftn::Length(this->get_pos(), sides.at(i).second)-this->get_size());
				Corpse::CollisionResponse(polygon, this, vector_response);
				return;

			}
		}
    }
}

float Circle::get_size() const { return this->size; }

ftn::Rectangle Circle::get_corpse_bounds() const { return ftn::Rectangle({sf::Vector2f(this->get_pos_x()-this->get_size(), this->get_pos_y()-this->get_size()), sf::Vector2f(this->get_size() * 2.0f, this->get_size() * 2.0f)}); }

}