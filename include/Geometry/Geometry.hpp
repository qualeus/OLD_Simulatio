#ifndef Geometry_HPP
#define Geometry_HPP

#include "Bounds.hpp"
#include "Config.hpp"
#include "Maths.hpp"
#include "Text.hpp"
#include "Vector.hpp"
#include "Vertices.hpp"

namespace gmt {

#ifndef PRECISION_SET
#define PRECISION_SET
typedef float PHYSICS_PRECISION;
#endif

#define MIN_DAMPING 0.001
#define MAX_DAMPING 100
#define MAX_GRAVITY_DISTANCE 1000000000 /* To avoid strange behaviours when comparing big numbers */

using UnitI = PHYSICS_PRECISION;
using VectorI = Vector<PHYSICS_PRECISION>;
using BoundsI = Bounds<PHYSICS_PRECISION>;
using VerticesI = Vertices<PHYSICS_PRECISION>;
using CollisionI = Collision<PHYSICS_PRECISION>;
using TextI = Text<PHYSICS_PRECISION>;

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
/*
struct Rectangle {
    sf::Vector2f pos;
    sf::Vector2f size;
};
*/
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
// float Dot(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
// float Dot(float x1, float y1, float x2, float y2);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// float Cross(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
// float Cross(float x1, float y1, float x2, float y2);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// sf::Vector2f Pow(const sf::Vector2f &vect, int power);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// float Length(const sf::Vector2f &vect);
// float Length(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);
// float Length(float x, float y);
// float Length(float x1, float y1, float x2, float y2);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// sf::Vector2f Normalize(const sf::Vector2f &vect);
// sf::Vector2f Norme(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);
// sf::Vector2f inverse_Norme(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// sf::Vector2f Rotate_Point(sf::Vector2f pA, sf::Vector2f pB, float angle);
// void Rotate(sf::Vector2f &vect, int angle);
// void Scale(sf::Vector2f &vect, float scale);
// bool Lines_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// int line_orientation(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C);
// bool on_segment(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C);
// bool Segments_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D);
// bool Test_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// sf::Vector2f Line_Projection(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b);
/* /!\ AB => C changé en A => BC /!\ */
// sf::Vector2f Segment_Projection(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b, const sf::Vector2f &vect_c);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// std::pair<int, sf::Vector2f> Line_Circle_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const float &size);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// gmt::BoundsI Reorder_Rectangle(gmt::BoundsI rectangle);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// float bearing(float x1, float y1, float x2, float y2);
// float angle(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// bool rect_in_bounds(const gmt::BoundsI &object, const gmt::BoundsI &limits);
// bool rect_out_bounds(const gmt::BoundsI &object, const gmt::BoundsI &limits);

/* ======================================= */
/*           DONE : TO REIMPLEMENT         */
/* ======================================= */
// sf::Vector2f Mirrored_Point(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B);

}  // namespace gmt
#endif