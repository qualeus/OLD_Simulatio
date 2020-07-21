#ifndef Quadtree_HPP
#define Quadtree_HPP

#include <vector>
#include <memory>

#include "../include/Functional.hpp"
#include "../include/Circle.hpp"
#include "../include/Polygon.hpp"

namespace phy {

#define MAX_OBJECT 1
#define MAX_LEVELS 10

class Quadtree
{
private:
	int level;
	struct vtr::Rectangle bounds;

	std::shared_ptr<Quadtree> node_A;
	std::shared_ptr<Quadtree> node_B;
	std::shared_ptr<Quadtree> node_C;
	std::shared_ptr<Quadtree> node_D;

	std::vector<std::shared_ptr<Corpse>> corpses;
	
public:
	Quadtree(vtr::Rectangle bounds = vtr::Rectangle({sf::Vector2f(), sf::Vector2f()}), int level=1);
	virtual ~Quadtree();

	void clear();
	void split();
	int get_index(std::shared_ptr<Corpse> corpse);
	void insert(std::shared_ptr<Corpse> corpse);

	std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> make_pairs();

	std::shared_ptr<Quadtree> get_node(int i);

	std::vector<vtr::Rectangle> get_all_bounds();
};

}

#endif