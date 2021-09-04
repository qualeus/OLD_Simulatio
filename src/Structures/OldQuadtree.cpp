#include "../../include/Structures/OldQuadtree.hpp"

namespace gmt {

/* Vectors Operators: Quadtree=Copy */
// TODO: is this function ever used? (Not a real copy, no link to the upper tree)
template <typename T>
Quadtree<T>& Quadtree<T>::operator=(const Quadtree<T>& rhs) {
    this->level = rhs.level;
    this->bounds = rhs.bounds;

    for (int i = 0; i < NUMBER_SUB; i++) { this->get_node(i) = nullptr; }
    this->corpses = std::vector<std::shared_ptr<phy::Corpse>>();
    return *this;
}
template Quadtree<int>& Quadtree<int>::operator=(const Quadtree<int>& rhs);
template Quadtree<float>& Quadtree<float>::operator=(const Quadtree<float>& rhs);
template Quadtree<double>& Quadtree<double>::operator=(const Quadtree<double>& rhs);

/* Builder Constructor */
template <typename T>
Quadtree<T>::Quadtree(const Bounds<T>& bounds, const int& level) {
    this->level = level;
    this->bounds = bounds;

    for (int i = 0; i < NUMBER_SUB; i++) { this->get_node(i) = nullptr; }
    this->corpses = std::vector<std::shared_ptr<phy::Corpse>>();

    // Problem, if can't allocatoe objects on bounds, keep it (so can be > than max)
    // if (level < MAX_LEVELS) { this->corpses.reserve(MAX_OBJECT); }
}
template Quadtree<int>::Quadtree(const Bounds<int>& bounds, const int& level);
template Quadtree<float>::Quadtree(const Bounds<float>& bounds, const int& level);
template Quadtree<double>::Quadtree(const Bounds<double>& bounds, const int& level);

/* Default Destructor */
template <typename T>
Quadtree<T>::~Quadtree() {}
template Quadtree<int>::~Quadtree();
template Quadtree<float>::~Quadtree();
template Quadtree<double>::~Quadtree();

/* Clear the nodes of the quadtree */
template <typename T>
void Quadtree<T>::Clear() {
    ClearNodes();
    // TODO ???, are we trying to delete all the corpses
    std::vector<std::shared_ptr<phy::Corpse>>().swap(this->corpses);
}
template void Quadtree<int>::Clear();
template void Quadtree<float>::Clear();
template void Quadtree<double>::Clear();

/* Clear each subnodes of the quadtree */
template <typename T>
void Quadtree<T>::ClearNodes() {
    for (int i = 0; i < NUMBER_SUB; i++) {
        if (get_node(i) != nullptr) {
            get_node(i)->Clear();
            set_node(i, nullptr);
        }
    }
}
template void Quadtree<int>::ClearNodes();
template void Quadtree<float>::ClearNodes();
template void Quadtree<double>::ClearNodes();

/*
    +---+---+
    | A | B | Clear the previous pointers and
    +---+---+ allocate new ones with the sub
    | C | D | Quadtrees in the four coordinates
    +---+---+
*/
template <typename T>
void Quadtree<T>::Split() {
    // Determine the SubBounds
    T x1 = this->bounds.x1;
    T y1 = this->bounds.y1;
    T x2 = this->bounds.x2;
    T y2 = this->bounds.y2;
    T midx = (x2 - x1) / T(2);
    T midy = (y2 - y1) / T(2);

    // Reset the Sub Nodes
    ClearNodes();

    this->node_A = std::make_shared<Quadtree<T>>(Quadtree<T>(Bounds<T>(x1, y1, x1 + midx, y1 + midy), this->level + 1));
    this->node_B = std::make_shared<Quadtree<T>>(Quadtree<T>(Bounds<T>(x1 + midx, y1, x2, y1 + midy), this->level + 1));
    this->node_C = std::make_shared<Quadtree<T>>(Quadtree<T>(Bounds<T>(x1, y2 + midy, x1 + midx, y2), this->level + 1));
    this->node_D = std::make_shared<Quadtree<T>>(Quadtree<T>(Bounds<T>(x1 + midx, y1 + midy, x2, y2), this->level + 1));
}
template void Quadtree<int>::Split();
template void Quadtree<float>::Split();
template void Quadtree<double>::Split();

/*
    Get the node where the object belong
    -1 = none; 0 = A; 1 = B; 2 = C: 3 = D
*/
template <typename T>
int Quadtree<T>::Index(std::shared_ptr<phy::Corpse> corpse) {
    int index = -1;

    BoundsI corpse_bounds = corpse->get_bounds();

    // Determine the SubBounds
    UnitI x1 = this->bounds.x1;
    UnitI y1 = this->bounds.y1;
    UnitI x2 = this->bounds.x2;
    UnitI y2 = this->bounds.y2;
    UnitI midx = (x2 - x1) / UnitI(2);
    UnitI midy = (y2 - y1) / UnitI(2);

    bool pos_top = corpse_bounds.y1 < midy && corpse_bounds.y2 < midy;
    bool pos_bot = corpse_bounds.y1 >= midy;
    bool pos_left = corpse_bounds.x1 < midx && corpse_bounds.x2 < midx;
    bool pos_right = corpse_bounds.x1 >= midx;

    if (pos_top) {
        if (pos_left) {
            index = 0;
        } else if (pos_right) {
            index = 1;
        }
    } else if (pos_bot) {
        if (pos_left) {
            index = 2;
        } else if (pos_right) {
            index = 3;
        }
    }
    return index;
}
template int Quadtree<int>::Index(std::shared_ptr<phy::Corpse> corpse);
template int Quadtree<float>::Index(std::shared_ptr<phy::Corpse> corpse);
template int Quadtree<double>::Index(std::shared_ptr<phy::Corpse> corpse);

/* Size of the array of corpses */
template <typename T>
int Quadtree<T>::Size() {
    return this->corpses.size();
}
template int Quadtree<int>::Size();
template int Quadtree<float>::Size();
template int Quadtree<double>::Size();

/*
    Insert the object into the Quadtree
    if the node excelm the capacity it will
    split and add all objects to subnodes
*/
template <typename T>
void Quadtree<T>::Insert(std::shared_ptr<phy::Corpse> corpse) {
    if (this->node_A != nullptr) {
        int index = Index(corpse);
        if (index != -1) {
            get_node(index)->Insert(corpse);
            return;
        }
    }

    this->corpses.push_back(corpse);

    if (this->corpses.size() > MAX_OBJECT && this->level < MAX_LEVELS) {
        if (this->node_A == nullptr) { Split(); }

        int i = 0;
        while (i < this->corpses.size()) {
            int index = Index(this->corpses[i]);
            if (index != -1) {
                std::shared_ptr<phy::Corpse> rem = remove_return(i, this->corpses);
                get_node(index)->Insert(rem);
            } else {
                i++;
            }
        }
    }
}
template void Quadtree<int>::Insert(std::shared_ptr<phy::Corpse> corpse);
template void Quadtree<float>::Insert(std::shared_ptr<phy::Corpse> corpse);
template void Quadtree<double>::Insert(std::shared_ptr<phy::Corpse> corpse);

/*
    Recursively clear all the unused Quadtree to gain performances
    return true: empty
    return false: filled
*/
template <typename T>
bool Quadtree<T>::ClearEmpty() {
    // If not the smallest quadtree, test the sub quadtrees
    if (!(this->node_A == nullptr && this->node_B == nullptr && this->node_C == nullptr && this->node_D == nullptr)) {
        for (int i = 0; i < NUMBER_SUB; i++) {
            if (get_node(i) != nullptr) {
                if (get_node(i)->ClearEmpty()) {
                    get_node(i)->Clear();
                    set_node(i, nullptr);
                }
            }
        }
    }

    // Test after update if it's the smallest
    if (this->node_A == nullptr && this->node_B == nullptr && this->node_C == nullptr && this->node_D == nullptr) {
        if (this->corpses.size() == 0) { return true; }
    }
    return false;
}
template bool Quadtree<int>::ClearEmpty();
template bool Quadtree<float>::ClearEmpty();
template bool Quadtree<double>::ClearEmpty();

/* return true if at least one of the sub quadtrees is not null */
template <typename T>
bool Quadtree<T>::NotNull() {
    if (this->node_A != nullptr) { return true; }
    if (this->node_B != nullptr) { return true; }
    if (this->node_C != nullptr) { return true; }
    if (this->node_D != nullptr) { return true; }
    return false;
}
template bool Quadtree<int>::NotNull();
template bool Quadtree<float>::NotNull();
template bool Quadtree<double>::NotNull();

/* Return the bodies pairs in the sub nodes */
template <typename T>
std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> Quadtree<T>::make_pairs() {
    std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> pairs = std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>>();
    int corpse_size = corpses.size();
    if (this->corpses.size() > 1) {
        for (int a = 0; a < corpse_size; a++) {
            for (int b = a + 1; b < corpse_size; b++) { pairs.push_back({this->corpses[a], this->corpses[b]}); }
        }
    }

    std::vector<std::shared_ptr<phy::Corpse>> sub_corpses = get_sub_corpses();
    for (int a = 0; a < corpse_size; a++) {
        for (int b = 0; b < sub_corpses.size(); b++) { pairs.push_back({this->corpses[a], sub_corpses[b]}); }
    }

    for (int i = 0; i < NUMBER_SUB; i++) {
        if (get_node(i) != nullptr) {
            std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> temp_pairs = get_node(i)->make_pairs();
            for (int j = 0; j < temp_pairs.size(); j++) { pairs.push_back(temp_pairs[j]); }
        }
    }
    return pairs;
}
template std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> Quadtree<int>::make_pairs();
template std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> Quadtree<float>::make_pairs();
template std::vector<std::pair<std::shared_ptr<phy::Corpse>, std::shared_ptr<phy::Corpse>>> Quadtree<double>::make_pairs();

/* Return all this corpses */
template <typename T>
std::vector<std::shared_ptr<phy::Corpse>> Quadtree<T>::get_sub_corpses() {
    std::vector<std::shared_ptr<phy::Corpse>> sub_corpses = std::vector<std::shared_ptr<phy::Corpse>>();

    for (int i = 0; i < NUMBER_SUB; i++) {
        if (get_node(i) != nullptr) {
            std::vector<std::shared_ptr<phy::Corpse>> temp_sub_corpses = get_node(i)->get_all_corpses();
            for (int j = 0; j < temp_sub_corpses.size(); j++) { sub_corpses.push_back(temp_sub_corpses[j]); }
        }
    }
    return sub_corpses;
}
template std::vector<std::shared_ptr<phy::Corpse>> Quadtree<int>::get_sub_corpses();
template std::vector<std::shared_ptr<phy::Corpse>> Quadtree<float>::get_sub_corpses();
template std::vector<std::shared_ptr<phy::Corpse>> Quadtree<double>::get_sub_corpses();

/* Return all the sub corpses */
template <typename T>
std::vector<std::shared_ptr<phy::Corpse>> Quadtree<T>::get_all_corpses() {
    std::vector<std::shared_ptr<phy::Corpse>> sub_corpses = std::vector<std::shared_ptr<phy::Corpse>>();

    for (int j = 0; j < this->corpses.size(); j++) { sub_corpses.push_back(this->corpses[j]); }

    for (int i = 0; i < NUMBER_SUB; i++) {
        if (get_node(i) != nullptr) {
            std::vector<std::shared_ptr<phy::Corpse>> temp_sub_corpses = get_node(i)->get_all_corpses();
            for (int j = 0; j < temp_sub_corpses.size(); j++) { sub_corpses.push_back(temp_sub_corpses[j]); }
        }
    }

    return sub_corpses;
}
template std::vector<std::shared_ptr<phy::Corpse>> Quadtree<int>::get_all_corpses();
template std::vector<std::shared_ptr<phy::Corpse>> Quadtree<float>::get_all_corpses();
template std::vector<std::shared_ptr<phy::Corpse>> Quadtree<double>::get_all_corpses();

/* Get the pointer to the Sub Quadtree at the given index */
template <typename T>
std::shared_ptr<Quadtree<T>> Quadtree<T>::get_node(int i) {
    switch (i) {
        case 0: return this->node_A;
        case 1: return this->node_B;
        case 2: return this->node_C;
        case 3: return this->node_D;
    }
    return nullptr;
}
template std::shared_ptr<Quadtree<int>> Quadtree<int>::get_node(int i);
template std::shared_ptr<Quadtree<float>> Quadtree<float>::get_node(int i);
template std::shared_ptr<Quadtree<double>> Quadtree<double>::get_node(int i);

/* Set the SubNode at the given index */
template <typename T>
void Quadtree<T>::set_node(int i, std::shared_ptr<Quadtree<T>> node) {
    switch (i) {
        case 0: this->node_A = node;
        case 1: this->node_B = node;
        case 2: this->node_C = node;
        case 3: this->node_D = node;
    }
}
template void Quadtree<int>::set_node(int i, std::shared_ptr<Quadtree<int>> node);
template void Quadtree<float>::set_node(int i, std::shared_ptr<Quadtree<float>> node);
template void Quadtree<double>::set_node(int i, std::shared_ptr<Quadtree<double>> node);

/* Get all the bounds of the Sub Quadtree */
template <typename T>
std::vector<Bounds<T>> Quadtree<T>::get_all_bounds() {
    std::vector<Bounds<T>> temp = std::vector<Bounds<T>>();

    temp.push_back(this->bounds);

    for (int i = 0; i < NUMBER_SUB; i++) {
        if (get_node(i) != nullptr) {
            std::vector<Bounds<T>> temp_bounds = get_node(i)->get_all_bounds();
            for (int j = 0; j < temp_bounds.size(); j++) { temp.push_back(temp_bounds[j]); }
        }
    }
    return temp;
}
template std::vector<Bounds<int>> Quadtree<int>::get_all_bounds();
template std::vector<Bounds<float>> Quadtree<float>::get_all_bounds();
template std::vector<Bounds<double>> Quadtree<double>::get_all_bounds();

}  // namespace gmt