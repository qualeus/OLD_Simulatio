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
    int LS = 100;  // 2,998 * 10e+8
    int G = 10;    // 6.7 * 10e-11

    int dt = 1000;
    int collision_accuracy = 10;
    int constraint_accuracy = 10;

    ftn::Rectangle limits;

   public:
    System(bool gravity = false, float force_x = 0.0f, float force_y = 0.0f, float limit_x = 4000.0f, float limit_y = 4000.0f);  // System Constructor
    virtual ~System();                                                                                                           // System Destructor

    void Prepare();

    void Step();
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

    int get_dt();
    void set_dt(int dt);
    void add_dt(int dt);

    float get_force_x();
    void set_force_x(float force_x);

    float get_force_y();
    void set_force_y(float force_y);

    bool get_gravity();
    void set_gravity(bool gravity);

    int get_LS();
    void set_LS(int LS);

    int get_G();
    void set_G(int G);

    int get_collision_accuracy();
    void set_collision_accuracy(int collision_accuracy);

    int get_constraint_accuracy();
    void set_constraint_accuracy(int constraint_accuracy);

    int get_corpses_size();
    int get_pairs_size();
    int get_quad_pairs_size();

    void addCorpse(Polygon a);
    void addCorpse(Circle a);

    void add_corpse(std::shared_ptr<Corpse> a);
    void add_pair(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b);

    ftn::Rectangle get_limits();
    void set_limits(ftn::Rectangle limits);

    std::vector<std::shared_ptr<Corpse>> get_corpses();
    std::shared_ptr<Corpse> get_corpse(int index);

    std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> get_quad_pair(int index);

    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> get_pairs();
    std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>> get_pair(int index);
    std::shared_ptr<Corpse> get_pair_A(int index);
    std::shared_ptr<Corpse> get_pair_B(int index);
};

}  // namespace phy

#endif