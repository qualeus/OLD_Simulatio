#include "../../include/Structures/QuadTree.hpp"

namespace gmt {

/* Vectors Operators: QuadTree=Copy */
template <typename T>
QuadTree<T>& QuadTree<T>::operator=(const QuadTree<T>& rhs) {
    return *this;
}
template QuadTree<int>& QuadTree<int>::operator=(const QuadTree<int>& rhs);
template QuadTree<float>& QuadTree<float>::operator=(const QuadTree<float>& rhs);
template QuadTree<double>& QuadTree<double>::operator=(const QuadTree<double>& rhs);

/* Builder Constructor */
template <typename T>
QuadTree<T>::QuadTree(const Bounds<T>& bounds, const int& max_count, const int& max_depth) {
    this->bounds = bounds;
    this->max_count = max_count;
    this->max_depth = max_depth;
    this->root = std::make_unique<QuadNode<T>>(QuadNode<T>());
}
template QuadTree<int>::QuadTree(const Bounds<int>& bounds, const int& max_count, const int& max_depth);
template QuadTree<float>::QuadTree(const Bounds<float>& bounds, const int& max_count, const int& max_depth);
template QuadTree<double>::QuadTree(const Bounds<double>& bounds, const int& max_count, const int& max_depth);

/* Default Destructor */
/*
template <typename T>
QuadTree<T>::~QuadTree() {}
template QuadTree<int>::~QuadTree();
template QuadTree<float>::~QuadTree();
template QuadTree<double>::~QuadTree();
*/

template <typename T>
void QuadTree<T>::SplitNode(const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& bounds) {
    // if (node == nullptr) { std::bad_weak_ptr(); }
    // if (!node->Leaf()) { std::invalid_argument(); }

    // Initialize the children nodes
    for (int i = 0; i < 4; i++) { node->childs[i] = std::make_unique<QuadNode<T>>(QuadNode<T>()); }

    // Assign the corpses to the childs
    std::vector<std::shared_ptr<phy::Corpse>> new_corpses = {};

    for (int i = 0; i < node->corpses.size(); i++) {
        BoundsI corpse_bounds;

        const std::shared_ptr<phy::Corpse> corpse = node->corpses.at(i);

        if (phy::Circle* circle = dynamic_cast<phy::Circle*>(corpse.get())) {
            corpse_bounds = circle->get_corpse_bounds();
        } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(corpse.get())) {
            corpse_bounds = polygon->get_corpse_bounds();
        }

        int index = Quadrant(bounds, corpse_bounds);

        if (index == -1) {
            new_corpses.push_back(corpse);
        } else {
            node->childs[index]->corpses.push_back(corpse);
        }
    }

    node->corpses = std::move(new_corpses);
}
template void QuadTree<int>::SplitNode(const std::unique_ptr<QuadNode<int>>& node, const Bounds<int>& bounds);
template void QuadTree<float>::SplitNode(const std::unique_ptr<QuadNode<float>>& node, const Bounds<float>& bounds);
template void QuadTree<double>::SplitNode(const std::unique_ptr<QuadNode<double>>& node, const Bounds<double>& bounds);

template <typename T>
Bounds<T> QuadTree<T>::NodeBounds(const Bounds<T>& bounds, int index) {
    T midx = (bounds.x2 - bounds.x1) / T(2);
    T midy = (bounds.y2 - bounds.y1) / T(2);
    switch (index) {
        case 0: return Bounds<T>(bounds.x1, bounds.y1, midx, midy);  // NW
        case 1: return Bounds<T>(midx, bounds.y1, bounds.x2, midy);  // NE
        case 2: return Bounds<T>(bounds.x1, midy, midx, bounds.y2);  // SW
        case 3: return Bounds<T>(midx, midy, bounds.x2, bounds.y2);  // SE
    }
    throw std::out_of_range();
}
template Bounds<int> QuadTree<int>::NodeBounds(const Bounds<int>& bounds, int index);
template Bounds<float> QuadTree<float>::NodeBounds(const Bounds<float>& bounds, int index);
template Bounds<double> QuadTree<double>::NodeBounds(const Bounds<double>& bounds, int index);

template <typename T>
int QuadTree<T>::Quadrant(const Bounds<T>& node_bounds, const Bounds<T>& corpse_bounds) {
    T midx = (node_bounds.x2 - node_bounds.x1) / T(2);
    T midy = (node_bounds.y2 - node_bounds.y1) / T(2);

    // bool pos_top = corpse_bounds.y1 < midy && corpse_bounds.y2 < midy;
    // bool pos_bot = corpse_bounds.y1 >= midy;
    bool pos_left = corpse_bounds.x1 < midx && corpse_bounds.x2 < midx;
    bool pos_right = corpse_bounds.x1 >= midx;

    int index = -1;  // The corpse intersect with the node bounds

    if (corpse_bounds.y1 < midy && corpse_bounds.y2 < midy) {
        if (pos_left) {
            index = 0;
        } else if (pos_right) {
            index = 1;
        }
    } else if (corpse_bounds.y1 >= midy) {
        if (pos_left) {
            index = 2;
        } else if (pos_right) {
            index = 3;
        }
    }

    return index;
}
template int QuadTree<int>::Quadrant(const Bounds<int>& node_bounds, const Bounds<int>& corpse_bounds);
template int QuadTree<float>::Quadrant(const Bounds<float>& node_bounds, const Bounds<float>& corpse_bounds);
template int QuadTree<double>::Quadrant(const Bounds<double>& node_bounds, const Bounds<double>& corpse_bounds);

template <typename T>
void QuadTree<T>::AddCorpse(std::shared_ptr<phy::Corpse> corpse) {
    BoundsI corpse_bounds;

    if (phy::Circle* circle = dynamic_cast<phy::Circle*>(corpse.get())) {
        corpse_bounds = circle->get_corpse_bounds();
    } else if (phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(corpse.get())) {
        corpse_bounds = polygon->get_corpse_bounds();
    }

    Add(0, root, bounds, corpse, corpse_bounds);
}
template void QuadTree<int>::AddCorpse(std::shared_ptr<phy::Corpse> corpse);
template void QuadTree<float>::AddCorpse(std::shared_ptr<phy::Corpse> corpse);
template void QuadTree<double>::AddCorpse(std::shared_ptr<phy::Corpse> corpse);

template <typename T>
void QuadTree<T>::Add(int depth, const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<T>& corpse_bounds) {
    // if (node == nullptr) { std::bad_weak_ptr(); }

    if (node->Leaf()) {
        if (depth > max_depth) {
            node->corpses.push_back(corpse);  // Can't descend further more
        } else if (node->corpses.size() > max_count) {
            node->corpses.push_back(corpse);  // Still room in the node
        } else {
            Split(node, node_bounds);
            Add(depth, node, node_bounds, corpse, corpse_bounds);
        }
    } else {
        int index = Quadrant(node_bounds, corpse_bounds);
        if (index == -1) {
            node->corpses.push_back(corpse);  // Can't fit the corpse in a sub node
        } else {
            Add(depth + 1, node->childs[index], NodeBounds(node_bounds, index), corpse, corpse_bounds);
        }
    }
}
template void QuadTree<int>::Add(int depth, const std::unique_ptr<QuadNode<int>>& node, const Bounds<int>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<int>& corpse_bounds);
template void QuadTree<float>::Add(int depth, const std::unique_ptr<QuadNode<float>>& node, const Bounds<float>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<float>& corpse_bounds);
template void QuadTree<double>::Add(int depth, const std::unique_ptr<QuadNode<double>>& node, const Bounds<double>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<double>& corpse_bounds);

template <typename T>
void QuadTree<T>::Remove(const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<T>& corpse_bounds) {
    // if (node == nullptr) { std::bad_weak_ptr(); }

    if (node->Leaf()) {
        SearchRemove(node, corpse);
    } else {
        int index = Quadrant(node_bounds, corpse_bounds);
        if (index == -1) {
            SearchRemove(node, corpse);
        } else {
            Remove(node->childs[index], NodeBounds(node_bounds, index), corpse, corpse_bounds);
        }
    }
}
template void QuadTree<int>::Remove(const std::unique_ptr<QuadNode<int>>& node, const Bounds<int>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<int>& corpse_bounds);
template void QuadTree<float>::Remove(const std::unique_ptr<QuadNode<float>>& node, const Bounds<float>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<float>& corpse_bounds);
template void QuadTree<double>::Remove(const std::unique_ptr<QuadNode<double>>& node, const Bounds<double>& node_bounds, std::shared_ptr<phy::Corpse> corpse, const Bounds<double>& corpse_bounds);

template <typename T>
void QuadTree<T>::SearchRemove(const std::unique_ptr<QuadNode<T>>& node, std::shared_ptr<phy::Corpse> corpse) {
    auto it = std::find_if(std::begin(node->corpses), std::end(node->corpses), [this, &corpse](const std::shared_ptr<phy::Corpse>& rhs) { return corpse == rhs; });

    if (it == std::end(node->corpses)) { std::out_of_range(); }  // trying to repove an object that don't exist in the node

    *it = std::move(node->corpses.back());
    node->corpses.pop_back();
}
template void QuadTree<int>::SearchRemove(const std::unique_ptr<QuadNode<int>>& node, std::shared_ptr<phy::Corpse> corpse);
template void QuadTree<float>::SearchRemove(const std::unique_ptr<QuadNode<float>>& node, std::shared_ptr<phy::Corpse> corpse);
template void QuadTree<double>::SearchRemove(const std::unique_ptr<QuadNode<double>>& node, std::shared_ptr<phy::Corpse> corpses);

template <typename T>
void QuadTree<T>::CleanupNode(const std::unique_ptr<QuadNode<T>>& node) {
    if (!node.Leaf()) {
        int leaf_count = 0;
        for (int i = 0; i < 4; i++) {
            if (node->childs[i].Leaf()) {
                leaf_count++;
            } else {
                CleanupNode(node->childs[i]);
            }
        }

        if (leaf_count == 4) { node.childs = {nullptr, nullptr, nullptr, nullptr}; }
    }
}
template void QuadTree<int>::CleanupNode(const std::unique_ptr<QuadNode<int>>& node);
template void QuadTree<float>::CleanupNode(const std::unique_ptr<QuadNode<float>>& node);
template void QuadTree<double>::CleanupNode(const std::unique_ptr<QuadNode<double>>& node);

template <typename T>
void QuadTree<T>::ClearNode(const std::unique_ptr<QuadNode<T>>& node) {
    node->corpses = {};
    if (!node.Leaf()) {
        for (int i = 0; i < 4; i++) {
            ClearNode(node->childs[i]);
            node->childs[i] = nullptr;
        }
    }
}
template void QuadTree<int>::ClearNode(const std::unique_ptr<QuadNode<int>>& node);
template void QuadTree<float>::ClearNode(const std::unique_ptr<QuadNode<float>>& node);
template void QuadTree<double>::ClearNode(const std::unique_ptr<QuadNode<double>>& node);

template <typename T>
void QuadTree<T>::FindPairs(const std::unique_ptr<QuadNode<T>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const {
    const int corpse_size = node->corpses.size();
    if (corpse_size > 1) {
        for (int a = 0; a < corpse_size; a++) {
            for (int b = a + 1; b < corpse_size; b++) { pairs.push_back({node->corpses.at(a), node->corpses.at(b)}); }
        }
    }

    if (!node.Leaf()) {
        for (int i = 0; i < 4; i++) {
            FindPairs(node->childs[i], pairs);                   // Find pairs between the Child corpses
            ChildsPairs(node->corpses, node->childs[i], pairs);  // Find pairs between the Child corpses and this node corpses
        }
    }
}
template void QuadTree<int>::FindPairs(const std::unique_ptr<QuadNode<int>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
template void QuadTree<float>::FindPairs(const std::unique_ptr<QuadNode<float>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
template void QuadTree<double>::FindPairs(const std::unique_ptr<QuadNode<double>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;

template <typename T>
void QuadTree<T>::FindBounds(const std::unique_ptr<QuadNode<T>>& node, const Bounds<T>& node_bounds, std::vector<Bounds<T>> bounds) const {
    bounds.push_back(node_bounds);
    if (!node.Leaf()) {
        for (int i = 0; i < 4; i++) { FindBounds(node->childs[i], NodeBounds(node_bounds, i), bounds); }
    }
}
template void QuadTree<int>::FindBounds(const std::unique_ptr<QuadNode<int>>& node, const Bounds<int>& node_bounds, std::vector<Bounds<int>> bounds) const;
template void QuadTree<float>::FindBounds(const std::unique_ptr<QuadNode<float>>& node, const Bounds<float>& node_bounds, std::vector<Bounds<float>> bounds) const;
template void QuadTree<double>::FindBounds(const std::unique_ptr<QuadNode<double>>& node, const Bounds<double>& node_bounds, std::vector<Bounds<double>> bounds) const;

template <typename T>
void QuadTree<T>::ChildsPairs(const std::vector<std::shared_ptr<phy::Corpse>>& corpses, const std::unique_ptr<QuadNode<T>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const {
    const int parent_size = corpses.size();
    const int corpse_size = node->corpses.size();
    if (parent_size > 0 && corpse_size > 0) {
        for (int a = 0; a < parent_size; a++) {
            for (int b = 0; b < corpse_size; b++) { pairs.push_back({corpses.at(a), node->corpses.at(b)}); }
        }
    }
}
template void QuadTree<int>::ChildsPairs(const std::vector<std::shared_ptr<phy::Corpse>>& corpses, const std::unique_ptr<QuadNode<int>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
template void QuadTree<float>::ChildsPairs(const std::vector<std::shared_ptr<phy::Corpse>>& corpses, const std::unique_ptr<QuadNode<float>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;
template void QuadTree<double>::ChildsPairs(const std::vector<std::shared_ptr<phy::Corpse>>& corpses, const std::unique_ptr<QuadNode<double>>& node, std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>& pairs) const;

template <typename T>
void QuadTree<T>::UpdateCorpse(const int& index) {
    // TODO
}
template void QuadTree<int>::UpdateCorpse(const int& index);
template void QuadTree<float>::UpdateCorpse(const int& index);
template void QuadTree<double>::UpdateCorpse(const int& index);

template <typename T>
void QuadTree<T>::RemoveCorpse(std::shared_ptr<phy::Corpse> corpse) {
    // TODO
}
template void QuadTree<int>::RemoveCorpse(std::shared_ptr<phy::Corpse> corpse);
template void QuadTree<float>::RemoveCorpse(std::shared_ptr<phy::Corpse> corpse);
template void QuadTree<double>::RemoveCorpse(std::shared_ptr<phy::Corpse> corpse);

template <typename T>
void QuadTree<T>::Update() {
    // TODO
}
template void QuadTree<int>::Update();
template void QuadTree<float>::Update();
template void QuadTree<double>::Update();

template <typename T>
void QuadTree<T>::Cleanup() {
    CleanupNode(root);
}
template void QuadTree<int>::Cleanup();
template void QuadTree<float>::Cleanup();
template void QuadTree<double>::Cleanup();

template <typename T>
void QuadTree<T>::Clear() {}
template void QuadTree<int>::Clear();
template void QuadTree<float>::Clear();
template void QuadTree<double>::Clear();

template <typename T>
std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> QuadTree<T>::ComputePairs() {
    std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> pairs = {};
    FindPairs(root, pairs);
    return pairs;
}
template std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> QuadTree<int>::ComputePairs();
template std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> QuadTree<float>::ComputePairs();
template std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> QuadTree<double>::ComputePairs();

template <typename T>
std::vector<Bounds<T>> QuadTree<T>::ComputeBounds() {
    std::vector<Bounds<T>> bounds = {};
    FindBounds(root, bounds);
    return bounds;
}
template std::vector<Bounds<int>> QuadTree<int>::ComputeBounds();
template std::vector<Bounds<float>> QuadTree<float>::ComputeBounds();
template std::vector<Bounds<double>> QuadTree<double>::ComputeBounds();

}  // namespace gmt