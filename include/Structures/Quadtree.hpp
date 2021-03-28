#ifndef QuadTree_HPP
#define QuadTree_HPP

#include "../Corpses/Circle.hpp"
#include "../Corpses/Corpse.hpp"
#include "../Corpses/Polygon.hpp"
#include "../Geometry/Bounds.hpp"
#include "../Geometry/Maths.hpp"
#include "../Geometry/Vector.hpp"
#include "QuadNode.hpp"

namespace gmt {

template <typename T>
class QuadTree {
   private:
    int max_count;
    int max_depth;
    Bounds<T> bounds;
    std::unique_ptr<QuadNode<T>> root;

    void SplitNode(const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& bounds);
    Bounds<T> NodeBounds(const Bounds<T>& bounds, int index);
    int Quadrant(const Bounds<T>& node_bounds, const Bounds<T>& corpse_bounds);

    void Add(int depth, const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<T>& corpse_bounds);
    void Remove(const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<T>& corpse_bounds);
    void SearchRemove(const std::unique_ptr<QuadNode<T>>& node, std::shared_ptr<phy::Corpse> corpse);
    void CleanupNode(const std::unique_ptr<QuadNode<T>>& node);
    void ClearNode(const std::unique_ptr<QuadNode<T>>& node);

    void FindPairs(const std::unique_ptr<QuadNode<T>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
    void ChildsPairs(const std::vector<std::shared_ptr<phy::Corpse>>& corpses, const std::unique_ptr<QuadNode<T>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
    void FindBounds(const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& node_bounds, std::vector<Bounds<T>> bounds) const;

   public:
    QuadTree<T>& operator=(const QuadTree<T>& rhs);
    QuadTree(const Bounds<T>& bounds, const int& max_count, const int& max_depth);
    // ~QuadTree();

    void Update();
    void Cleanup();
    void Clear();

    void AddCorpse(std::shared_ptr<phy::Corpse> corpse);
    void RemoveCorpse(std::shared_ptr<phy::Corpse> corpse);
    void UpdateCorpse(const int& index);

    std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> ComputePairs();

    std::vector<Bounds<T>> ComputeBounds();
};

}  // namespace gmt

#endif