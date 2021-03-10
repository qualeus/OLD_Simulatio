#ifndef System_HPP
#define System_HPP

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Config.hpp"
#include "Corpses/Circle.hpp"
#include "Corpses/Corpse.hpp"
#include "Corpses/Polygon.hpp"
#include "Geometry/Geometry.hpp"
#include "Geometry/Quadtree.hpp"

namespace phy {

#define AROUND_QUADTREE gmt::UnitI(1000)

class System {
   private:
    std::vector<std::shared_ptr<Corpse>> corpses;
    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> pairs;
    std::vector<std::pair<std::shared_ptr<Corpse>, std::shared_ptr<Corpse>>> quad_pairs;

    gmt::QuadtreeI quadtree;
    gmt::BoundsI limits;

    bool gravity;
    gmt::UnitI LS = 2.998e+8;  // 2,998 * 10e+8
    gmt::UnitI G = 1.6e-2;     // 6.7 * 10e-11

    gmt::UnitI force_x;
    gmt::UnitI force_y;

    gmt::UnitI dt = 1.0f;
    double t = 0.0;

    int collision_accuracy = 10;
    int constraint_accuracy = 10;

   public:
    System(bool gravity = false, gmt::UnitI force_x = 0.0f, gmt::UnitI force_y = 0.0f, gmt::UnitI limit_x = 4000.0f, gmt::UnitI limit_y = 4000.0f);  // System Constructor
    System& operator=(const System& rhs);                                                                                                            // System Copy
    virtual ~System();                                                                                                                               // System Destructor

    void Prepare();

    void Step();
    void UpdateTime();
    void CheckLimits();
    void CorpsesStep();
    void CorpseStop(int i);
    void PairsStep();
    void QuadPairsStep();

    void Gravity(std::shared_ptr<Corpse> a, std::shared_ptr<Corpse> b);

    void InitQuadtree();
    void StepQuadtree();
    std::shared_ptr<gmt::QuadtreeI> get_quadtree();

    gmt::UnitI get_dt() const;
    void set_dt(gmt::UnitI dt);

    double get_t() const;
    void set_t(double t);

    gmt::UnitI get_force_x() const;
    void set_force_x(gmt::UnitI force_x);

    gmt::UnitI get_force_y() const;
    void set_force_y(gmt::UnitI force_y);

    bool get_gravity() const;
    void set_gravity(bool gravity);

    gmt::UnitI get_LS() const;
    void set_LS(gmt::UnitI LS);

    gmt::UnitI get_G() const;
    void set_G(gmt::UnitI G);

    int get_collision_accuracy() const;
    void set_collision_accuracy(int collision_accuracy);

    int get_constraint_accuracy() const;
    void set_constraint_accuracy(int constraint_accuracy);

    int get_corpses_size() const;
    int get_pairs_size() const;
    int get_quad_pairs_size() const;

    void addCorpse(Polygon polygon);
    void addCorpse(Circle circle);

    void add_corpse(std::shared_ptr<Corpse> corpse);
    void add_pair(std::shared_ptr<Corpse> corpseA, std::shared_ptr<Corpse> corpseB);

    gmt::BoundsI get_limits() const;
    void set_limits(gmt::BoundsI limits);

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