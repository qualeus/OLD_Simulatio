#ifndef Polygon_HPP
#define Polygon_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Circle.hpp"
#include "Corpse.hpp"

namespace phy {

const static int ID_POLYGON = 3;

class Polygon : public Corpse {
   private:
    int points_number;
    std::vector<sf::Vector2f> points;
    std::vector<sf::Vector2f> relative_points;
    std::vector<std::vector<std::shared_ptr<sf::Vector2f>>> triangulation;

   public:
    Polygon(std::initializer_list<sf::Vector2f> points = {}, float mass = 1.0f, float damping = 1.0f, float speed_x = 0.0f, float speed_y = 0.0f, float rotation = 0.0f, float motor = 0.0f, bool fixed = false, bool tied = false, bool etherial = false, sf::Color color = sf::Color::White);
    Polygon& operator=(const Polygon& rhs);
    virtual ~Polygon();

    const int get_class();

    void Step();
    void Stop();
    void Move(float x, float y, bool relative = true);
    void Move(sf::Vector2f move, bool relative = true);
    bool inBounds(float x1, float x2, float y1, float y2);
    bool Pointed(float x, float y);

    void Collision(std::shared_ptr<Corpse> a);

    void triangulate();
    void update_points();

    std::vector<sf::Vector2f> init_relative_points(std::vector<sf::Vector2f> points);
    sf::Vector2f compute_center(std::vector<sf::Vector2f> points);
    gmt::Rectangle get_corpse_bounds() const;

    int get_points_number() const;
    void add_point(sf::Vector2f point);
    void remove_point(int i);

    std::vector<sf::Vector2f> get_points() const;
    void set_points(std::vector<sf::Vector2f> points);

    std::vector<sf::Vector2f> get_relative_points() const;
    void set_relative_points(std::vector<sf::Vector2f> relative_points);

    std::vector<float> get_sides_size() const;
    std::vector<sf::Vector2f> get_sides_val() const;
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> get_sides() const;
    std::vector<std::vector<std::shared_ptr<sf::Vector2f>>> get_triangulation() const;
    bool is_convex() const;
};

}  // namespace phy

#endif