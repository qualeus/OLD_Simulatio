#ifndef System_HPP
#define System_HPP

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "../include/Circle.hpp"
#include "../include/Corpse.hpp"
#include "../include/Functional.hpp"
#include "../include/Polygon.hpp"
#include "../include/Quadtree.hpp"

namespace phy {

#define AROUND_QUADTREE 1000
#define COLLISION_ACCURACY 10  // 50 // Number of times the collision step is done per frame

class System {
   private:
    std::vector<Circle> circles;
    std::vector<Polygon> polygons;

    int corpses_size = 0;
    int pairs_size = 0;
    std::vector<std::shared_ptr<Corpse>> corpses;
    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> pairs;
    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> quad_pairs;

    Quadtree quadtree;

    float force_x;
    float force_y;
    bool gravity;
    float LS = 2.998e+8;  // 2,998 * 10e+8
    float G = 1.6e-2;     // 6.7 * 10e-11

    float dt = 1.0f;
    double t = 0.0;

    int collision_accuracy = 10;
    int constraint_accuracy = 10;

    ftn::Rectangle limits;

   public:
    System(bool gravity = false, float force_x = 0.0f, float force_y = 0.0f, float limit_x = 4000.0f, float limit_y = 4000.0f);  // System Constructor
    System& operator=(const System& rhs);                                                                                        // System Copy
    virtual ~System();                                                                                                           // System Destructor

    void Prepare();

    void Step();
    void UpdateTime();
    void CheckLimits();
    void CorpsesStep();
    void CorpseStop(int i);
    void PairsStep();
    void QuadPairsStep();

    void Collision(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b);
    void Forces(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b);

    void InitQuadtree();
    void StepQuadtree();
    std::shared_ptr<Quadtree> get_quadtree();

    float get_dt() const;
    void set_dt(float dt);

    double get_t() const;
    void set_t(double t);

    float get_force_x() const;
    void set_force_x(float force_x);

    float get_force_y() const;
    void set_force_y(float force_y);

    bool get_gravity() const;
    void set_gravity(bool gravity);

    float get_LS() const;
    void set_LS(float LS);

    float get_G() const;
    void set_G(float G);

    int get_collision_accuracy() const;
    void set_collision_accuracy(int collision_accuracy);

    int get_constraint_accuracy() const;
    void set_constraint_accuracy(int constraint_accuracy);

    int get_corpses_size() const;
    int get_pairs_size() const;
    int get_quad_pairs_size() const;

    void addCorpse(Polygon a);
    void addCorpse(Circle a);

    void add_corpse(std::shared_ptr<Corpse> a);
    void add_pair(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b);

    ftn::Rectangle get_limits() const;
    void set_limits(ftn::Rectangle limits);

    std::vector<std::shared_ptr<Corpse>> get_corpses() const;
    std::shared_ptr<Corpse> get_corpse(int index) const;

    std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> get_quad_pair(int index) const;

    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> get_pairs() const;
    std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> get_pair(int index) const;
    std::shared_ptr<Corpse> get_pair_A(int index) const;
    std::shared_ptr<Corpse> get_pair_B(int index) const;
};

}  // namespace phy

#endif