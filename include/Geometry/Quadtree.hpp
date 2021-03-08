#ifndef Quadtree_HPP
#define Quadtree_HPP

#include <memory>
#include <vector>

#include "../Corpses/Circle.hpp"
#include "../Corpses/Polygon.hpp"
#include "Config.hpp"
#include "Geometry.hpp"

namespace gmt {

#define MAX_OBJECT 5
#define MAX_LEVELS 10

#define NUMBER_SUB 4

class Quadtree {
   private:
    int level;
    struct Rectangle bounds;

    std::shared_ptr<Quadtree> node_A;
    std::shared_ptr<Quadtree> node_B;
    std::shared_ptr<Quadtree> node_C;
    std::shared_ptr<Quadtree> node_D;

    std::vector<std::shared_ptr<phy::Corpse>> corpses;

   public:
    Quadtree(Rectangle bounds = Rectangle({sf::Vector2f(), sf::Vector2f()}), int level = 1);
    Quadtree& operator=(const Quadtree& rhs);
    virtual ~Quadtree();

    void clear();
    void clear_nodes();
    void split();
    int get_index(std::shared_ptr<phy::Corpse> corpse);
    int get_size();

    void insert(std::shared_ptr<phy::Corpse> corpse);
    bool clear_empty();
    bool sub_not_null();

    int get_level() const;
    void set_level(int level);

    Rectangle get_bounds() const;
    void set_bounds(Rectangle bounds);

    std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> make_pairs();

    std::vector<std::shared_ptr<phy::Corpse>> get_sub_corpses();
    std::vector<std::shared_ptr<phy::Corpse>> get_all_corpses();

    std::shared_ptr<Quadtree> get_node(int i);
    void set_node(int i, std::shared_ptr<Quadtree> node);

    std::vector<Rectangle> get_all_bounds();
};

}  // namespace gmt

#endif