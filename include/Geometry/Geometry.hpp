#ifndef Geometry_HPP
#define Geometry_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Config.hpp"

#define PI 3.141592653589793  /* Archimede Constant */
#define E 2.718281828459045   /* Euler Number */
#define TAU 6.283185307179586 /* Tau as 2PI */
#define GR 1.618033988749894  /* Golder Ratio */
#define PC 1.414213562373095  /* Pythagoras Constant */

#define EDGE_APPROXIMATION 1 /* TO DELETE */

namespace gmt {

template <typename T>
class Vector;

template <typename T>
class Bounds;

template <typename T>
class Text;

using unitI = PHYSICS_PRECISION;
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

bool Equals(float a, float b, float sigma = 0.1f);

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

float degree_to_radian(float degree);
float radian_to_degree(float radian);

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

float digits_comma(float number, int digits);

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

template <class C>
std::shared_ptr<C> remove(int i, std::vector<std::shared_ptr<C>> &vect) {
    std::shared_ptr<C> corpse = vect.at(i);
    vect.erase(vect.begin() + i);
    return corpse;
}

template <typename T>
std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename T>
std::string to_string(std::vector<T> vector) {
    std::ostringstream oss;
    oss << "{";
    if (!vector.empty()) {
        std::copy(vector.begin(), vector.end() - 1, std::ostream_iterator<T>(oss, ","));
        oss << vector.back();
    }
    oss << "}";
    return oss.str();
}

template <typename T>
std::string to_string(sf::Vector2<T> vector) {
    std::ostringstream oss;
    oss << "{" << vector.x << ";" << vector.y << "}";
    return oss.str();
}

template <typename T>
bool decimal_equals(const T &a, const T &b, T epsilon = std::numeric_limits<T>::epsilon()) {
    return fabs(a - b) < epsilon;
}

}  // namespace gmt
#endif