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

bool Polygon::inBounds(float x1, float x2, float y1, float y2) { return true; }
bool Polygon::Pointed(float x, float y) {
	// Ray Casting Algorithm
	std::vector<float> sizes = this->get_sides_size();
	float ray_lenght = ftn::Length(this->get_pos()-sf::Vector2f(x, y)) + *std::max_element(std::begin(sizes), std::end(sizes));

	sf::Vector2f rayA = {x, y};
	sf::Vector2f rayB = {x+ray_lenght, y}; //ftn::Normalize(this->get_pos()-sf::Vector2f(x, y))*ray_lenght;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides = get_sides();
	int intersections = 0;
	for (int i=0; i<sides.size(); i++) { if(ftn::Segments_Intersect(rayA, rayB, sides.at(i).first, sides.at(i).second)) { intersections++; } }
	if ((intersections & 1) == 1) { return true; }
	return false;
}

void Polygon::Collision(std::shared_ptr<Corpse> a) { }

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
