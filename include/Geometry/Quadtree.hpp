#ifndef Quadtree_HPP
#define Quadtree_HPP

#include "Maths.hpp"

namespace gmt {

#define MAX_OBJECT 5
#define MAX_LEVELS 10

#define NUMBER_SUB 4

template <typename T>
class Quadtree {
   public:
    int level;
    struct Bounds<T> bounds;
    std::shared_ptr<Quadtree<T>> node_A;
    std::shared_ptr<Quadtree<T>> node_B;
    std::shared_ptr<Quadtree<T>> node_C;
    std::shared_ptr<Quadtree<T>> node_D;
    std::vector<std::shared_ptr<phy::Corpse>> corpses;

    Quadtree<T>& operator=(const Quadtree<T>& rhs);

    Quadtree(const Bounds<T>& bounds = Bounds<T>(), const int& level = 1);
    ~Quadtree();

    void Clear();
    void ClearNodes();
    void Split();
    int Index(std::shared_ptr<phy::Corpse> corpse);
    int Size();

    void Insert(std::shared_ptr<phy::Corpse> corpse);
    bool ClearEmpty();
    bool NotNull();

    std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> make_pairs();
    std::vector<std::shared_ptr<phy::Corpse>> get_sub_corpses();
    std::vector<std::shared_ptr<phy::Corpse>> get_all_corpses();
    std::shared_ptr<Quadtree<T>> get_node(int i);
    void set_node(int i, std::shared_ptr<Quadtree<T>> node);
    std::vector<Bounds<T>> get_all_bounds();
};

}  // namespace gmt

#endif