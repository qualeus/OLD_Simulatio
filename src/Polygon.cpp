#include "../include/Polygon.hpp"

namespace phy {

Polygon::Polygon(std::vector<sf::Vector2f> points, float mass, float damping, float speed_x, float speed_y, bool fixed, bool etherial, sf::Color color):Corpse(compute_center(points).x, compute_center(points).y, mass, damping, fixed, etherial, color) {
	this->last_pos = sf::Vector2f(compute_center(points).x-speed_x, compute_center(points).y-speed_y);
	this->points_number = points.size();
	this->points = points;
	this->relative_points = init_relative_points(points, compute_center(points));
}

Polygon::~Polygon() {}

const int Polygon::get_class() { return ID_POLYGON;}

void Polygon::Move(float x, float y, bool relative) { 
	if (relative) {
		this->current_pos = this->current_pos+sf::Vector2f(x, y);
	} else {
		this->current_pos = sf::Vector2f(x, y);
	}
	this->update_points();
}
void Polygon::Move(sf::Vector2f move, bool relative) {
	if (relative) {
		this->current_pos = this->current_pos+move; 
	} else {
		this->current_pos = move; 
	}
	this->update_points();
}

bool Polygon::inBounds(float x1, float x2, float y1, float y2) {
	return true;
}
bool Polygon::Pointed(float x, float y) {
	// Ray Casting Algorithm
	std::vector<float> sizes = this->get_sides_size();
	float ray_lenght = ftn::Length(this->get_pos()-sf::Vector2f(x, y)) + *std::max_element(std::begin(sizes), std::end(sizes));

	sf::Vector2f rayA = {x, y};
	sf::Vector2f rayB = {x+ray_lenght, y}; //ftn::Normalize(this->get_pos()-sf::Vector2f(x, y))*ray_lenght;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = this->get_sides();
	int intersections = 0;
	for (int i=0; i<sides.size(); i++) { if(ftn::Segments_Intersect(rayA, rayB, sides.at(i).first, sides.at(i).second)) { intersections++; } }
	if ((intersections & 1) == 1) { return true; }
	return false;
}

void Polygon::Collision(std::shared_ptr<Corpse> a) {
	if (Circle* circle = dynamic_cast<Circle*>(a.get())) {

		// Polygon / Circle collision
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = this->get_sides();

		for (int i=0; i<sides.size(); i++) {
			auto test_intersect = ftn::Line_Circle_Intersect(sides.at(i).first, sides.at(i).second, circle->get_pos(), circle->get_size());
			
			// Collide if one side of the polygon intersect with the circle 
			if (test_intersect.first) {
				bool asymetric = this->get_fixed() || circle->get_fixed();		
				float damping = (this->get_bounce() + circle->get_bounce()) * 0.5f;

				float normal_mass = this->get_mass() + circle->get_mass();
				float normal_mass_this = this->get_mass() / normal_mass;
				float normal_mass_circle = circle->get_mass() / normal_mass;
				if (asymetric) {
					if (!this->get_fixed()) {
						this->Move(sf::Vector2f(test_intersect.second.x, test_intersect.second.y) * damping * normal_mass_circle);
					} else if (!circle->get_fixed()) {
						circle->Move(-sf::Vector2f(test_intersect.second.x, test_intersect.second.y) * damping * normal_mass_this);
					} else {
						this->Move(sf::Vector2f(test_intersect.second.x, test_intersect.second.y) * 0.5f * damping * normal_mass_circle);
						circle->Move(-sf::Vector2f(test_intersect.second.x, test_intersect.second.y) * 0.5f * damping * normal_mass_this);
					}
				} else {
					this->Move(sf::Vector2f(test_intersect.second.x, test_intersect.second.y) * 0.5f * damping * normal_mass_circle);
					circle->Move(-sf::Vector2f(test_intersect.second.x, test_intersect.second.y) * 0.5f * damping * normal_mass_this);
				}
				return;
			}
		}

		// Collide if the center of the circle is in the polygon
		if (this->Pointed(circle->get_pos().x, circle->get_pos().y)) {
			bool asymetric = this->get_fixed() || circle->get_fixed();
			float damping = (this->get_bounce() + circle->get_bounce()) * 0.5f;

			float normal_mass = this->get_mass() + circle->get_mass();
			float normal_mass_this = this->get_mass() / normal_mass;
			float normal_mass_circle = circle->get_mass() / normal_mass;
			
			sf::Vector2f pos_diff = this->get_pos()-circle->get_pos();
			if (asymetric) {
				if (!this->get_fixed()) {
					this->Move(-pos_diff * damping * normal_mass_circle);
				} else if (!circle->get_fixed()) {
					circle->Move(pos_diff * damping * normal_mass_this);
				} else {
					this->Move(-pos_diff * 0.5f * damping * normal_mass_circle);
					circle->Move(pos_diff * 0.5f * damping * normal_mass_this);
				}
			} else {
				this->Move(-pos_diff * 0.5f * damping * normal_mass_circle);
				circle->Move(pos_diff * 0.5f * damping * normal_mass_this);
			}	
		}

	} else if (Polygon* polygon = dynamic_cast<Polygon*>(a.get())) {

		// Polygon / Polygon collision (Separating axis theorem)

		// Make separating axis (perpendicular to the line that pass by the two objects center)
		sf::Vector2f axis = ftn::Norme(this->get_pos(), polygon->get_pos()); // sf::Vector2f axis = ftn::Norme(this->get_diff_pos(), polygon->get_diff_pos());
		
		// Find the two farthest points of the projection points on the separator axis
		std::vector<float> self_projections = std::vector<float>();
		std::vector<float> other_projections = std::vector<float>();

		for (int i=0; i<this->get_points_number(); i++) { self_projections.push_back(ftn::Dot(this->get_points().at(i), axis)); }
		for (int i=0; i<polygon->get_points_number(); i++) { other_projections.push_back(ftn::Dot(polygon->get_points().at(i), axis)); }

		const auto self_minmax = std::minmax_element(self_projections.begin(), self_projections.end());
		const auto other_minmax = std::minmax_element(other_projections.begin(), other_projections.end());
		
		float self_min = *self_minmax.first;
		float self_max = *self_minmax.second;
		float other_min = *other_minmax.first;
		float other_max = *other_minmax.second;

	}
}

void Polygon::update_points() {
	for (int i=0; i<this->points_number; i++) { this->points.at(i) = this->get_pos() + this->relative_points.at(i); }
}

std::vector<sf::Vector2f> Polygon::init_relative_points(std::vector<sf::Vector2f> points, sf::Vector2f pos) {
	std::vector<sf::Vector2f> relative_points = std::vector<sf::Vector2f>();
	for (int i=0; i<points.size(); i++) { relative_points.push_back(points.at(i)-pos); }
	return relative_points;
}

void Polygon::set_points(std::vector<sf::Vector2f> points) { this->points = points; }

void Polygon::add_point(int i) {}
void Polygon::remove_point(int i) {}

sf::Vector2f Polygon::compute_center(std::vector<sf::Vector2f> points) {
	sf::Vector2f points_average = sf::Vector2f(0.0f, 0.0f);
	if (points.size() == 0) { return points_average; }

	for (int i=0; i<points.size(); i++) { points_average = points_average + points.at(i); }
	return points_average / (float)this->points_number;
}


void Polygon::Step() {
	if (this->fixed)  {
		this->last_pos = this->current_pos;
	} else {
		sf::Vector2f diff_pos = this->current_pos - this->last_pos;
		this->last_pos = this->current_pos;
		this->current_pos = this->current_pos + diff_pos;
	}
	this->update_points();
}
void Polygon::Stop() { this->last_pos = this->current_pos; }

ftn::Rectangle Polygon::get_corpse_bounds() const {
	const std::vector<sf::Vector2f> points = this->points;
	const auto min_max_x = std::minmax_element(points.begin(), points.end(), [](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.x < rhs.x; });
	const auto min_max_y = std::minmax_element(points.begin(), points.end(), [](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.y < rhs.y; });

	float min_x = (*min_max_x.first).x;
	float max_x = (*min_max_x.second).x;
	float min_y = (*min_max_y.first).y;
	float max_y = (*min_max_y.second).y;

	return {sf::Vector2f(min_x, min_y), sf::Vector2f(max_x-min_x, max_y-min_y)}; 
}

int Polygon::get_points_number() const { return this->points_number; }
std::vector<sf::Vector2f> Polygon::get_points() const { return this->points; }

std::vector<float> Polygon::get_sides_size() const { 
	std::vector<sf::Vector2f> sides = this->get_sides_val();
	std::vector<float> sizes = std::vector<float>();
	for (int i=0; i < sides.size(); i++) { sizes.push_back(ftn::Length(sides.at(i))); }
	return sizes;
}
std::vector<sf::Vector2f> Polygon::get_sides_val() const {
	std::vector<sf::Vector2f> sides = std::vector<sf::Vector2f>();
	if (this->points_number > 1) {
		for (int i=0; i<this->points_number-1; i++) { sides.push_back(this->points.at(i+1)-this->points.at(i)); }
		sides.push_back(this->points.at(0)-this->points.at(this->points_number-1));
	}
	return sides;
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Polygon::get_sides() const {
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> pairs = std::vector<std::pair<sf::Vector2f, sf::Vector2f>>();
	if (this->points_number > 1) {
		for (int i=0; i<this->points_number-1; i++) { pairs.push_back({this->points.at(i),this->points.at(i+1)}); }
		pairs.push_back({this->points.at(this->points_number-1),this->points.at(0)});
	}
	return pairs;
}
}
