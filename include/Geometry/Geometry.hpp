#ifndef Geometry_HPP
#define Geometry_HPP

#include "Bounds.hpp"
#include "Config.hpp"
#include "Maths.hpp"
#include "Text.hpp"
#include "Vector.hpp"

#define EDGE_APPROXIMATION 1 /* TO DELETE */

namespace gmt {

template <typename T>
class Vector;

template <typename T>
class Bounds;

template <typename T>
class Text;

using UnitI = PHYSICS_PRECISION;
using VectorI = Vector<PHYSICS_PRECISION>;
using BoundsI = Bounds<PHYSICS_PRECISION>;
using TextI = Text<PHYSICS_PRECISION>;

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
struct Rectangle {
    sf::Vector2f pos;
    sf::Vector2f size;
};
/*
struct Text {
    std::string str = "";
    float x = 0.0f;
    float y = 0.0f;
    float size = 18;
    bool fixed = false;
    sf::Color color = sf::Color::White;
};
*/

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
float Dot(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
float Dot(float x1, float y1, float x2, float y2);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
float Cross(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
float Cross(float x1, float y1, float x2, float y2);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
sf::Vector2f Pow(const sf::Vector2f &vect, int power);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
float Length(const sf::Vector2f &vect);
float Length(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);
float Length(float x, float y);
float Length(float x1, float y1, float x2, float y2);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
sf::Vector2f Normalize(const sf::Vector2f &vect);
sf::Vector2f Norme(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);
sf::Vector2f inverse_Norme(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
sf::Vector2f Rotate_Point(sf::Vector2f pA, sf::Vector2f pB, float angle);
void Rotate(sf::Vector2f &vect, int angle);
void Scale(sf::Vector2f &vect, float scale);
bool Lines_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
int line_orientation(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C);
bool on_segment(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C);
bool Segments_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D);
bool Test_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
sf::Vector2f Line_Projection(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
/* /!\ AB => C changé en A => BC /!\ */
sf::Vector2f Segment_Projection(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b, const sf::Vector2f &vect_c);

sf::Vector2f Centroid(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges);
sf::Vector2f Points_Average(std::vector<sf::Vector2f> points);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
std::pair<int, sf::Vector2f> Line_Circle_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const float &size);

std::pair<sf::Vector2f, sf::Vector2f> Closest_Edge(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> sides, sf::Vector2f point);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
gmt::Rectangle Reorder_Rectangle(gmt::Rectangle rectangle);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
float bearing(float x1, float y1, float x2, float y2);
float angle(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
bool rect_in_bounds(const gmt::Rectangle &object, const gmt::Rectangle &limits);
bool rect_out_bounds(const gmt::Rectangle &object, const gmt::Rectangle &limits);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
sf::Vector2f Mirrored_Point(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);

}  // namespace gmt
#endif