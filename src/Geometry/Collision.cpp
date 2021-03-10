#include "../../include/Geometry/Collision.hpp"

namespace gmt {

template <typename T>
void Collision<T>::Resolve(std::shared_ptr<phy::Corpse> lhs, std::shared_ptr<phy::Corpse> rhs) {
    /* Left Hand Side pointer classes */
    phy::Circle* lcircle;
    phy::Polygon* lpolygon;

    /* Right Hand Side pointer classes */
    phy::Circle* rcircle;
    phy::Polygon* rpolygon;

    /* Id classes */
    int lhs_class = -1;
    int rhs_class = -1;

    if (lcircle = dynamic_cast<phy::Circle*>(lhs.get())) {
        lhs_class = lcircle.get_class();
    } else if (lpolygon = dynamic_cast<phy::Polygon*>(lhs.get())) {
        rhs_class = lpolygon.get_class()
    }

    if (rcircle = dynamic_cast<phy::Circle*>(rhs.get())) {
        lhs_class = rcircle.get_class();
    } else if (rpolygon = dynamic_cast<phy::Polygon*>(rhs.get())) {
        rhs_class = rpolygon.get_class()
    }

    /* Possible to factorise but it's easier to read like that */
    if (lhs_class == Circle::id_class() && rhs_class == Circle::id_class()) {
        Collision<T>::CircleOnCircle(rcircle, lcircle);
    } else if (lhs_class == Circle::id_class() && rhs_class == Polygon::id_class()) {
        Collision<T>::CircleOnPolygon(rcircle, lpolygon);
    } else if (lhs_class == Polygon::id_class() && rhs_class == Circle::id_class()) {
        Collision<T>::CircleOnPolygon(lcircle, rpolygon);
    } else if (lhs_class == Polygon::id_class() && rhs_class == Polygon::id_class()) {
        Collision<T>::PolygonOnPolygon(rpolygon, lpolygon);
    }
}
template void Collision<int>::Resolve(std::shared_ptr<phy::Corpse> lhs, std::shared_ptr<phy::Corpse> rhs);
template void Collision<float>::Resolve(std::shared_ptr<phy::Corpse> lhs, std::shared_ptr<phy::Corpse> rhs);
template void Collision<double>::Resolve(std::shared_ptr<phy::Corpse> lhs, std::shared_ptr<phy::Corpse> rhs);

/* Circle / Circle Collision */
template <typename T>
void Collision<T>::CircleOnCircle(phy::Circle* circleA, phy::Circle* circleB) {
    T distance = gmt::Vector<T>::Distance(this->get_pos(), circle->get_pos());
    T overlap = (this->get_size() + circle->get_size() - distance);

    if (overlap < 0) { return; }  // Not colliding

    T x_diff = this->get_pos_x() - circle->get_pos_x();
    T y_diff = this->get_pos_y() - circle->get_pos_y();

    Vector<T> vector_response = gmt::Vector<T>(x_diff / distance, y_diff / distance) * overlap;
    Collision<T>::CollisionResponse(circleA, circleB, vector_response);
}
template void Collision<int>::CircleOnCircle(phy::Circle* circleA, phy::Circle* circleB);
template void Collision<float>::CircleOnCircle(phy::Circle* circleA, phy::Circle* circleB);
template void Collision<double>::CircleOnCircle(phy::Circle* circleA, phy::Circle* circleB);

/* Circle / Polygon collision */
template <typename T>
void Collision<T>::CircleOnPolygon(phy::Circle* circle, phy::Polygon* polygon) {
    std::vector<std::pair<gmt::VectorI, gmt::VectorI>> sides = polygon->get_sides();

    // Collide if the center of the circle is in the polygon
    if (polygon->Pointed(this->get_pos().x, this->get_pos().y)) {
        // Find the closest point on edges
        std::pair<gmt::VectorI, gmt::VectorI> closest_side = gmt::Closest_Edge(sides, this->get_pos());

        gmt::VectorI closest_projection = gmt::Segment_Projection(closest_side.first, closest_side.second, this->get_pos());
        gmt::VectorI vector_response = gmt::Normalize(gmt::Norme(closest_side.second, closest_side.first)) * (gmt::Length(this->get_pos(), closest_projection) + this->get_size());
        Corpse::CollisionResponse(polygon, this, vector_response);
        return;
    }

    // Collide if one side of the polygon intersect with the circle

    for (int i = 0; i < sides.size(); i++) {
        auto test_intersect = gmt::VectorI::LineCercleIntersect(sides.at(i).first, sides.at(i).second, this->get_pos(), this->get_size());

        // Don't collide with any edge
        if (test_intersect.first == 0) { continue; }

        if (test_intersect.first == 1) {
            // Collide at the middle of an edge
            gmt::VectorI vector_response = (this->get_pos() - test_intersect.second).Normalize() * (gmt::VectorI::Distance(this->get_pos(), test_intersect.second) - this->get_size());
            Corpse::CollisionResponse(polygon, this, vector_response);
            return;

        } else if (test_intersect.first == 2) {
            // Collide with the first point of the edge (current edge + last edge)
            int last_edge = (i - 1) % sides.size();
            gmt::VectorI normals_average = gmt::VectorI::Normal(sides.at(last_edge).first, sides.at(last_edge).second) + gmt::VectorI::Normal(sides.at(i).first, sides.at(i).second);
            gmt::VectorI vector_response = normals_average.Normalize() * (gmt::VectorI::Distance(this->get_pos(), sides.at(i).first) - this->get_size());
            Corpse::CollisionResponse(polygon, this, vector_response);
            return;

        } else if (test_intersect.first == 3) {
            // Collide with the second point of the edge (current edge + next edge)
            int next_edge = (i + 1) % sides.size();
            gmt::VectorI normals_average = gmt::VectorI::Normal(sides.at(i).first, sides.at(i).second) + gmt::VectorI::Normal(sides.at(next_edge).first, sides.at(next_edge).second);
            gmt::VectorI vector_response = normals_average.Normalize() * (gmt::VectorI::Distance(this->get_pos(), sides.at(i).second) - this->get_size());
            Corpse::CollisionResponse(polygon, this, vector_response);
            return;
        }
    }
}

template void Collision<int>::CircleOnPolygon(phy::Circle* circle, phy::Polygon* polygon);
template void Collision<float>::CircleOnPolygon(phy::Circle* circle, phy::Polygon* polygon);
template void Collision<double>::CircleOnPolygon(phy::Circle* circle, phy::Polygon* polygon);

template <typename T>
void Collision<T>::PolygonOnPolygon(phy::Polygon* polygonA, phy::Polygon* polygonB) {
    // TODO
}
template void Collision<int>::PolygonOnPolygon(phy::Polygon* polygonA, phy::Polygon* polygonB);
template void Collision<float>::PolygonOnPolygon(phy::Polygon* polygonA, phy::Polygon* polygonB);
template void Collision<double>::PolygonOnPolygon(phy::Polygon* polygonA, phy::Polygon* polygonB);

template <typename T>
void Collision<T>::Response(phy::Corpse* lhs, phy::Corpse* rhs, const Vector<T>& normal) {
    if (corpse_a->get_etherial()) { return; }
    if (corpse_b->get_etherial()) { return; }
    gmt::UnitI damping = (corpse_a->get_bounce() + corpse_b->get_bounce()) / gmt::UnitI(2);  // Damping is evenly distributed among the corpses

    /*
        TEMP: TODO
        - recompute the damping with the proper forces applied
        - push the corpse out of the other shape with the normal vector and the force passed by parameter (vect_force)
            => BUT DON'T CHANCE THE VELOCITY (how to do? last pos?)
        - Then apply the vector (2m2v¯2 + (m1 −m2)v¯1)/(2m2v¯2 + (m1 −m2)v¯1)

    */
    // Test if the collision is asymetric (Fixed/Not Fixed) / or if the two corpses are Fixed
    if (corpse_a->get_fixed() || corpse_b->get_fixed()) {
        if (!corpse_a->get_fixed()) {
            // corpse_a is Not Fixed and corpse_b is Fixed:
            // gmt::VectorI temp_pos = corpse_a->get_last_pos();

            // corpse_a->set_last_pos(corpse_a->get_pos());
            corpse_a->Move(vect_force);
            // corpse_a->Move(gmt::Mirrored_Point(temp_pos - corpse_a->get_pos(), vect_force) * damping);

        } else if (!corpse_b->get_fixed()) {
            // corpse_a is Fixed and corpse_b is Not Fixed:
            // gmt::VectorI temp_pos = corpse_b->get_last_pos();

            // corpse_b->set_last_pos(corpse_b->get_pos());
            corpse_b->Move(-vect_force);
            // corpse_b->Move(gmt::Mirrored_Point(temp_pos - corpse_b->get_pos(), vect_force) * damping);
        } else {
            // Both corpse_a and corpse_b are Fixed:
            // corpse_a->Move(force*0.5f);
            // corpse_b->Move(-force*0.5f);
        }
    } else {
        // Both corpse_a and corpse_b are Not Fixed:
        gmt::UnitI normal_mass = corpse_a->get_mass() + corpse_b->get_mass();
        gmt::UnitI normal_mass_corpse_a = corpse_a->get_mass() / normal_mass;
        gmt::UnitI normal_mass_corpse_b = corpse_b->get_mass() / normal_mass;

        // gmt::VectorI diff_pos_a = corpse_a->get_diff_pos();
        // gmt::VectorI diff_pos_b = corpse_b->get_diff_pos();

        // corpse_a->set_last_pos(corpse_a->get_pos());
        // corpse_b->set_last_pos(corpse_b->get_pos());

        corpse_a->Move(vect_force * normal_mass_corpse_b / gmt::UnitI(2));
        corpse_b->Move(-vect_force * normal_mass_corpse_a / gmt::UnitI(2));
        /*
        // Distance between corpses
        gmt::UnitI fDistance = gmt::Length(corpse_a->get_pos_x(), corpse_a->get_pos_y(), corpse_b->get_pos_x(), corpse_b->get_pos_y());

        // Normal
        gmt::UnitI nx = (corpse_b->get_pos_x() - corpse_a->get_pos_x()) / fDistance;
        gmt::UnitI ny = (corpse_b->get_pos_y() - corpse_a->get_pos_y()) / fDistance;

        // Tangent
        gmt::UnitI tx = -nx;
        gmt::UnitI ty = ny;

        // Dot product Tangent
        gmt::UnitI dpTanA = diff_pos_a.x * tx + diff_pos_a.y * ty;
        gmt::UnitI dpTanB = diff_pos_b.x * tx + diff_pos_b.y * ty;

        // Dot product Normal
        gmt::UnitI dpNormA = diff_pos_a.x * nx + diff_pos_a.y * ny;
        gmt::UnitI dpNormB = diff_pos_b.x * nx + diff_pos_b.y * ny;

        // Conservation of momentum
        gmt::UnitI mA = (dpNormA * (corpse_a->get_mass() - corpse_b->get_mass()) + 2.0f * corpse_b->get_mass() * dpNormB) / (corpse_a->get_mass() + corpse_b->get_mass());
        gmt::UnitI mB = (dpNormB * (corpse_b->get_mass() - corpse_a->get_mass()) + 2.0f * corpse_a->get_mass() * dpNormA) / (corpse_a->get_mass() + corpse_b->get_mass());

        corpse_a->Move(tx * dpTanA + nx * mA, ty * dpTanA + ny * mA);
        corpse_b->Move(tx * dpTanB + nx * mB, ty * dpTanB + ny * mB);
        */

        // corpse_a->Move(gmt::Mirrored_Point(temp_pos_a - corpse_a->get_pos(), vect_force) * damping);
        // corpse_b->Move(gmt::Mirrored_Point(temp_pos_b - corpse_b->get_pos(), vect_force) * damping);
    }
}

template void Collision<int>::Response(phy::Corpse* lhs, phy::Corpse* rhs, const Vector<int>& normal);
template void Collision<float>::Response(phy::Corpse* lhs, phy::Corpse* rhs, const Vector<float>& normal);
template void Collision<double>::Response(phy::Corpse* lhs, phy::Corpse* rhs, const Vector<double>& normal);

}  // namespace gmt