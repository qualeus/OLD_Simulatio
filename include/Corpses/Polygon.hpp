#ifndef Polygon_HPP
#define Polygon_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Circle.hpp"
#include "Config.hpp"
#include "Corpse.hpp"

namespace phy {

const static int ID_POLYGON = 3;

class Polygon : public Corpse {
   private:
    int points_number;
    std::vector<gmt::VectorI> points;
    std::vector<gmt::VectorI> relative_points;
    std::vector<std::vector<std::shared_ptr<gmt::VectorI>>> triangulation;

   public:
    Polygon(std::initializer_list<gmt::VectorI> points = {}, gmt::UnitI mass = gmt::UnitI(1), gmt::UnitI damping = gmt::UnitI(1), gmt::UnitI speed_x = gmt::UnitI(0), gmt::UnitI speed_y = gmt::UnitI(0), gmt::UnitI rotation = gmt::UnitI(0), gmt::UnitI motor = gmt::UnitI(0),
            gmt::VectorI propulsor = gmt::VectorI(), bool fixed = false, bool tied = false, bool etherial = false, sf::Color color = sf::Color::White);
    Polygon& operator=(const Polygon& rhs);
    virtual ~Polygon();

    const int get_class();

    void Step();
    void Stop();
    void Move(gmt::UnitI x, gmt::UnitI y, bool relative = true);
    void Move(gmt::VectorI move, bool relative = true);
    bool inBounds(gmt::UnitI x1, gmt::UnitI x2, gmt::UnitI y1, gmt::UnitI y2);
    bool Pointed(gmt::UnitI x, gmt::UnitI y);

    void Collision(std::shared_ptr<Corpse> a);

    void triangulate();
    void update_points();

    std::vector<gmt::VectorI> init_relative_points(std::vector<gmt::VectorI> points);
    gmt::VectorI compute_center(std::vector<gmt::VectorI> points);
    gmt::BoundsI get_corpse_bounds() const;

    int get_points_number() const;
    void add_point(gmt::VectorI point);
    void remove_point(int i);

    std::vector<gmt::VectorI> get_points() const;
    void set_points(std::vector<gmt::VectorI> points);

    std::vector<gmt::VectorI> get_relative_points() const;
    void set_relative_points(std::vector<gmt::VectorI> relative_points);

    std::vector<gmt::UnitI> get_sides_size() const;
    std::vector<gmt::VectorI> get_sides_val() const;
    std::vector<std::pair<gmt::VectorI, gmt::VectorI>> get_sides() const;
    std::vector<std::vector<std::shared_ptr<gmt::VectorI>>> get_triangulation() const;
    bool is_convex() const;
};

}  // namespace phy

#endif