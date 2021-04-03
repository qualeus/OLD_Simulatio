#ifndef QuadTree_HPP
#define QuadTree_HPP

#include "../Corpses/Circle.hpp"
#include "../Corpses/Corpse.hpp"
#include "../Corpses/Polygon.hpp"
#include "../Geometry/Bounds.hpp"
#include "../Geometry/Maths.hpp"
#include "../Geometry/String.hpp"
#include "../Geometry/Vector.hpp"
#include "QuadNode.hpp"

namespace gmt {

class QuadTree {
   private:
    void SplitNode(QuadNode* node, const BoundsI& bounds);
    BoundsI NodeBounds(const BoundsI& bounds, int index) const;
    BoundsI CorpseBounds(const std::shared_ptr<phy::Corpse>& corpse) const;
    int Quadrant(const BoundsI& node_bounds, const BoundsI& corpse_bounds) const;

    void Add(int depth, QuadNode* node, const BoundsI& node_bounds, std::shared_ptr<phy::Corpse> corpse, const BoundsI& corpse_bounds);
    void Remove(QuadNode* node, const BoundsI& node_bounds, std::shared_ptr<phy::Corpse> corpse, const BoundsI& corpse_bounds);
    void SearchRemove(QuadNode* node, std::shared_ptr<phy::Corpse> corpse);
    void CleanupNode(QuadNode* node);
    void ClearNode(QuadNode* node);

    void FindPairs(QuadNode* node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
    void ChildsPairs(const std::vector<std::shared_ptr<phy::Corpse>>& corpses, QuadNode* node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
    void FindBounds(QuadNode* node, const BoundsI& node_bounds, std::vector<BoundsI>& bounds) const;

   public:
    int max_count;
    int max_depth;
    BoundsI bounds;
    std::unique_ptr<QuadNode> root;
    QuadTree& operator=(const QuadTree& rhs);
    QuadTree(const BoundsI& bounds = BoundsI(), const int& max_count = 12, const int& max_depth = 10);
    QuadTree(const QuadTree& quadtree);
    // ~QuadTree();

    void Update();
    std::vector<gmt::BoundsI> AscendingUpdate(QuadNode* node, QuadNode* parent, const gmt::BoundsI& bounds);
    void Cleanup();
    void Clear();

    void DescendingUpdate(int depth, QuadNode* node, const gmt::BoundsI& node_bounds);

    void AddCorpse(std::shared_ptr<phy::Corpse> corpse);
    void RemoveCorpse(std::shared_ptr<phy::Corpse> corpse);
    void UpdateCorpse(std::shared_ptr<phy::Corpse> corpse);

    std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> ComputePairs() const;
    std::vector<BoundsI> ComputeBounds() const;
};

}  // namespace gmt

#endif