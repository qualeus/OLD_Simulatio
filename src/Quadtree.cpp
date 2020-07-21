#include "../include/Quadtree.hpp"

namespace phy {

Quadtree::Quadtree(vtr::Rectangle bounds, int level) {
	this->level = level;
	this->bounds = bounds;
	this->node_A = nullptr;
	this->node_B = nullptr;
	this->node_C = nullptr;
	this->node_D = nullptr;

	this->corpses = std::vector<std::shared_ptr<Corpse>>();
}
Quadtree::~Quadtree() {}

void Quadtree::clear() {
	if (this->node_A != nullptr) {
		this->node_A->clear();
		this->node_A = nullptr;
	}
	if (this->node_B != nullptr) {
		this->node_B->clear();
		this->node_B = nullptr;
	}
	if (this->node_C != nullptr) {
		this->node_C->clear();
		this->node_C = nullptr;
	}
	if (this->node_D != nullptr) {
		this->node_D->clear();
		this->node_D = nullptr;
	}

	std::vector<std::shared_ptr<Corpse>>().swap(this->corpses);
}

void Quadtree::clear_nodes() {
	if (this->node_A != nullptr) {
		this->node_A->clear();
		this->node_A = nullptr;
	}
	if (this->node_B != nullptr) {
		this->node_B->clear();
		this->node_B = nullptr;
	}
	if (this->node_C != nullptr) {
		this->node_C->clear();
		this->node_C = nullptr;
	}
	if (this->node_D != nullptr) {
		this->node_D->clear();
		this->node_D = nullptr;
	}
}

/*
	+---+---+ 
	| A | B | Clear the previous pointers and 
	+---+---+ allocate new ones with the sub
	| C | D | Quadtrees in the four coordinates
	+---+---+
*/
void Quadtree::split() {
	float pos_x = this->bounds.pos.x;
	float pos_y = this->bounds.pos.y;
	float sub_w = this->bounds.size.x / 2.0f;
	float sub_h = this->bounds.size.y / 2.0f;

	clear_nodes();
	this->node_A = std::make_shared<Quadtree>(Quadtree({sf::Vector2f(pos_x, pos_y), sf::Vector2f(sub_w, sub_h)}, this->level + 1));
	this->node_B = std::make_shared<Quadtree>(Quadtree({sf::Vector2f(pos_x+sub_w, pos_y), sf::Vector2f(sub_w, sub_h)}, this->level + 1));
	this->node_C = std::make_shared<Quadtree>(Quadtree({sf::Vector2f(pos_x, pos_y+sub_h), sf::Vector2f(sub_w, sub_h)}, this->level + 1));
	this->node_D = std::make_shared<Quadtree>(Quadtree({sf::Vector2f(pos_x+sub_w, pos_y+sub_h), sf::Vector2f(sub_w, sub_h)}, this->level + 1));
}

/*
	Get the node where the object belong
	-1 = none; 0 = A; 1 = B; 2 = C: 3 = D
*/
int Quadtree::get_index(std::shared_ptr<Corpse> corpse) {
	int index = -1;
	float mid_x = this->bounds.pos.x + (this->bounds.size.x / 2.0f);
	float mid_y = this->bounds.pos.y + (this->bounds.size.y / 2.0f);

	vtr::Rectangle corpse_bounds;
	
	if (Circle* circle = dynamic_cast<Circle*>(corpse.get())) {
		corpse_bounds = circle->get_corpse_bounds();
    } else if (Polygon* polygon = dynamic_cast<Polygon*>(corpse.get())) {
    	corpse_bounds = polygon->get_corpse_bounds();
    }

	bool pos_top = corpse_bounds.pos.y < mid_y && corpse_bounds.pos.y + corpse_bounds.size.y < mid_y;
	bool pos_bot = corpse_bounds.pos.y >= mid_y;
	bool pos_left = corpse_bounds.pos.x < mid_x && corpse_bounds.pos.x + corpse_bounds.size.x < mid_x;
	bool pos_right = corpse_bounds.pos.x >= mid_x;		
	
    if (pos_top) {
    	if (pos_left) { index = 0; } else if (pos_right) { index = 1; }
    } else if (pos_bot) {
    	if (pos_left) { index = 2; } else if (pos_right) { index = 3; }
    }
	
	return index;
}

/*
	Insert the object into the Quadtree
	if the node excelm the capacity it will
	split and add all objects to subnodes
*/ 
void Quadtree::insert(std::shared_ptr<Corpse> corpse) {
	if (this->node_A != nullptr) {
		int index = get_index(corpse);
		if (index != -1) {
			get_node(index)->insert(corpse);
			return;
		}
	}

	this->corpses.push_back(corpse);

	if (this->corpses.size() > MAX_OBJECT && this->level < MAX_LEVELS) {

		if (this->node_A == nullptr) { split(); }

		int i = 0;
		while (i < this->corpses.size()) {
			int index = get_index(this->corpses.at(i));
			if (index != -1) { 
				std::shared_ptr<Corpse> rem = vtr::remove(i, this->corpses);
				get_node(index)->insert(rem); 
			} else {
			 	i++; 
			}
		}
	}

}

std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> Quadtree::make_pairs() {

	std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> pairs = std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>>();
	int corpse_size = corpses.size();
	if (this->corpses.size() > 1) { 
		for (int a = 0; a < corpse_size; a++) {
			for (int b = a + 1; b < corpse_size; b++) { pairs.push_back({this->corpses.at(a), this->corpses.at(b)}); } 
		}
	}	

	std::vector<std::shared_ptr<Corpse>> sub_corpses = get_sub_corpses();
	for (int a = 0; a < corpse_size; a++) {
		for (int b = 0; b < sub_corpses.size(); b++) { pairs.push_back({this->corpses.at(a), sub_corpses.at(b)}); } 
	}

	for (int i = 0; i < NUMBER_SUB; i++) {
		if (get_node(i) != nullptr) {
			std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> temp_pairs = get_node(i)->make_pairs();
			for (int j = 0; j < temp_pairs.size(); j++) { pairs.push_back(temp_pairs.at(j)); }
		}
	}

	return pairs;
}

std::vector<std::shared_ptr<Corpse>> Quadtree::get_sub_corpses() {

	std::vector<std::shared_ptr<Corpse>> sub_corpses = std::vector<std::shared_ptr<Corpse>>();

	for (int i = 0; i < NUMBER_SUB; i++) {
		if (get_node(i) != nullptr) {
			std::vector<std::shared_ptr<Corpse>> temp_sub_corpses = get_node(i)->get_all_corpses();
			for (int j = 0; j < temp_sub_corpses.size(); j++) { sub_corpses.push_back(temp_sub_corpses.at(j)); }
		}
	}
	return sub_corpses;
}

std::vector<std::shared_ptr<Corpse>> Quadtree::get_all_corpses() {
	std::vector<std::shared_ptr<Corpse>> sub_corpses = std::vector<std::shared_ptr<Corpse>>();

	for (int j = 0; j < this->corpses.size(); j++) { sub_corpses.push_back(this->corpses.at(j)); }

	for (int i = 0; i < NUMBER_SUB; i++) {
		if (get_node(i) != nullptr) {
			std::vector<std::shared_ptr<Corpse>> temp_sub_corpses = get_node(i)->get_all_corpses();
			for (int j = 0; j < temp_sub_corpses.size(); j++) { sub_corpses.push_back(temp_sub_corpses.at(j)); }
		}
	}

	return sub_corpses;

}

std::shared_ptr<Quadtree> Quadtree::get_node(int i) {
	switch (i) {
		case 0: return this->node_A;
		case 1: return this->node_B;
		case 2: return this->node_C;
		case 3: return this->node_D;
	}

	return nullptr;
}

std::vector<vtr::Rectangle> Quadtree::get_all_bounds() {
	std::vector<vtr::Rectangle> temp = std::vector<vtr::Rectangle>();
	
	temp.push_back(this->bounds);

	for (int i = 0; i < NUMBER_SUB; i++) {
		if (get_node(i) != nullptr) {
			std::vector<vtr::Rectangle> temp_bounds = get_node(i)->get_all_bounds();
			for (int j = 0; j < temp_bounds.size(); j++) { temp.push_back(temp_bounds.at(j)); }
		}
	}
	return temp;
}

}