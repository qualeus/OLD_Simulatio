#ifndef Serialization_HPP
#define Serialization_HPP

// objects
#include <Constraints/Constraint.hpp>
#include <Corpses/Circle.hpp>
#include <Corpses/Corpse.hpp>
#include <Geometry/Bounds.hpp>
#include <Geometry/Vector.hpp>
#include <Geometry/Vertices.hpp>
#include <Structures/System.hpp>

// serialization
#include <cereal/access.hpp>
#include <cereal/details/traits.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

// inheritances declarations
CEREAL_REGISTER_TYPE(phy::Corpse)
CEREAL_REGISTER_TYPE(phy::Circle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(phy::Corpse, phy::Circle)

namespace cereal {

template <class Archive>
void save(Archive& archive, const phy::System& system) {
    std::vector<std::shared_ptr<phy::Corpse>> corpses = system.get_corpses();

    std::vector<std::shared_ptr<phy::Constraint>> constraints = system.get_constraints();
    gmt::BoundsI limits = system.get_limits();
    bool gravity = system.get_gravity();
    bool enable_limits = system.get_enable_limits();
    gmt::UnitI LS = system.get_LS();
    gmt::UnitI G = system.get_G();
    gmt::UnitI force_x = system.get_force_x();
    gmt::UnitI force_y = system.get_force_y();
    gmt::UnitI dt = system.get_dt();
    double t = system.get_t();
    int collision_accuracy = system.get_collision_accuracy();
    int constraint_accuracy = system.get_constraint_accuracy();

    archive(cereal::make_nvp<std::vector<std::shared_ptr<phy::Corpse>>>("corpses", corpses),              // param corpses
            cereal::make_nvp<std::vector<std::shared_ptr<phy::Constraint>>>("constraints", constraints),  // param constraints
            cereal::make_nvp<gmt::BoundsI>("limits", limits),                                             // param limits
            cereal::make_nvp<bool>("gravity", gravity),                                                   // param gravity
            cereal::make_nvp<bool>("enable_limits", enable_limits),                                       // param enable_limits
            cereal::make_nvp<gmt::UnitI>("LS", LS),                                                       // param LS
            cereal::make_nvp<gmt::UnitI>("G", G),                                                         // param G
            cereal::make_nvp<gmt::UnitI>("force_x", force_x),                                             // param force_x
            cereal::make_nvp<gmt::UnitI>("force_y", force_y),                                             // param force_y
            cereal::make_nvp<gmt::UnitI>("dt", dt),                                                       // param dt
            cereal::make_nvp<double>("t", t),                                                             // param t
            cereal::make_nvp<int>("collision_accuracy", collision_accuracy),                              // param collision_accuracy
            cereal::make_nvp<int>("constraint_accuracy", constraint_accuracy));                           // param constraint_accuracy
}

template <class Archive>
void load(Archive& archive, phy::System& system) {
    std::vector<std::shared_ptr<phy::Corpse>> corpses;
    std::vector<std::shared_ptr<phy::Constraint>> constraints;
    gmt::BoundsI limits;
    bool gravity;
    bool enable_limits;
    gmt::UnitI LS;
    gmt::UnitI G;
    gmt::UnitI force_x;
    gmt::UnitI force_y;
    gmt::UnitI dt;
    double t;
    int collision_accuracy;
    int constraint_accuracy;

    archive(cereal::make_nvp<std::vector<std::shared_ptr<phy::Corpse>>>("corpses", corpses),              // param corpses
            cereal::make_nvp<std::vector<std::shared_ptr<phy::Constraint>>>("constraints", constraints),  // param constraints
            cereal::make_nvp<gmt::BoundsI>("limits", limits),                                             // param limits
            cereal::make_nvp<bool>("gravity", gravity),                                                   // param gravity
            cereal::make_nvp<bool>("enable_limits", enable_limits),                                       // param enable_limits
            cereal::make_nvp<gmt::UnitI>("LS", LS),                                                       // param LS
            cereal::make_nvp<gmt::UnitI>("G", G),                                                         // param G
            cereal::make_nvp<gmt::UnitI>("force_x", force_x),                                             // param force_x
            cereal::make_nvp<gmt::UnitI>("force_y", force_y),                                             // param force_y
            cereal::make_nvp<gmt::UnitI>("dt", dt),                                                       // param dt
            cereal::make_nvp<double>("t", t),                                                             // param t
            cereal::make_nvp<int>("collision_accuracy", collision_accuracy),                              // param collision_accuracy
            cereal::make_nvp<int>("constraint_accuracy", constraint_accuracy));                           // param constraint_accuracy

    for (int i = 0; i < corpses.size(); i++) {
        system.add_corpse(corpses[i]);  // Check for shrd_ptr conversion type !
    }

    for (int i = 0; i < constraints.size(); i++) {
        system.add_constraint(constraints[i]);  // Check for shrd_ptr conversion type !
    }

    system.set_limits(limits);
    system.set_gravity(gravity);
    system.set_enable_limits(enable_limits);
    system.set_LS(LS);
    system.set_G(G);
    system.set_force_x(force_x);
    system.set_force_y(force_y);
    system.set_dt(dt);
    system.set_t(t);
    system.set_collision_accuracy(collision_accuracy);
    system.set_constraint_accuracy(constraint_accuracy);
}

template <class Archive>
void save(Archive& archive, const phy::Corpse& corpse) {
    gmt::VectorI current_pos = corpse.get_pos();
    archive(cereal::make_nvp<gmt::VectorI>("current_pos", current_pos));  // param current_pos
}

template <class Archive>
void load(Archive& archive, phy::Corpse& corpse) {
    gmt::VectorI current_pos;
    archive(cereal::make_nvp<gmt::VectorI>("current_pos", current_pos));  // param current_pos
    corpse.set_pos(current_pos);
}

template <class Archive>
void save(Archive& archive, const phy::Circle& circle) {
    gmt::UnitI size = circle.get_size();
    archive(cereal::base_class<phy::Corpse>(&circle));
    archive(cereal::make_nvp<gmt::UnitI>("size", size));  // param size
}

template <class Archive>
void load(Archive& archive, phy::Circle& circle) {
    gmt::UnitI size;
    archive(cereal::base_class<phy::Corpse>(&circle));
    archive(cereal::make_nvp<gmt::UnitI>("size", size));  // param size
    circle.set_size(size);
}

template <class Archive>
void save(Archive& archive, const phy::Constraint& constraint) {
    std::shared_ptr<phy::Corpse> corpse_a = constraint.get_corpse_a();
    std::shared_ptr<phy::Corpse> corpse_b = constraint.get_corpse_b();

    archive(cereal::make_nvp<std::shared_ptr<phy::Corpse>>("corpse_a", corpse_a),   // param corpse_a
            cereal::make_nvp<std::shared_ptr<phy::Corpse>>("corpse_b", corpse_b));  // param corpse_b
}

template <class Archive>
void load(Archive& archive, phy::Constraint& constraint) {
    std::shared_ptr<phy::Corpse> corpse_a;
    std::shared_ptr<phy::Corpse> corpse_b;

    archive(cereal::make_nvp<std::shared_ptr<phy::Corpse>>("corpse_a", corpse_a),   // param corpse_a
            cereal::make_nvp<std::shared_ptr<phy::Corpse>>("corpse_b", corpse_b));  // param corpse_b

    constraint.set_corpse_a(corpse_a);
    constraint.set_corpse_b(corpse_b);
}

template <class Archive, typename T>
void save(Archive& archive, const gmt::Vertices<T>& vertices) {
    archive(cereal::make_nvp<std::vector<std::shared_ptr<gmt::Vector<T>>>>("vertices", vertices.vertices));  // param vertices
}

template <class Archive, typename T>
void load(Archive& archive, gmt::Vertices<T>& vertices) {
    archive(cereal::make_nvp<std::vector<std::shared_ptr<gmt::Vector<T>>>>("vertices", vertices.vertices));  // param vertices
}

template <class Archive, typename T>
void save(Archive& archive, const gmt::Vector<T>& vector) {
    archive(cereal::make_nvp<T>("x", vector.x),   // param x
            cereal::make_nvp<T>("y", vector.y));  // param y
}

template <class Archive, typename T>
void load(Archive& archive, gmt::Vector<T>& vector) {
    archive(cereal::make_nvp<T>("x", vector.x),   // param x
            cereal::make_nvp<T>("y", vector.y));  // param y
}

template <class Archive, typename T>
void save(Archive& archive, const gmt::Bounds<T>& bounds) {
    archive(cereal::make_nvp<T>("x1", bounds.x1),   // param x1
            cereal::make_nvp<T>("y1", bounds.y1),   // param y1
            cereal::make_nvp<T>("x2", bounds.x2),   // param x2
            cereal::make_nvp<T>("y2", bounds.y2));  // param y2
}

template <class Archive, typename T>
void load(Archive& archive, gmt::Bounds<T>& bounds) {
    archive(cereal::make_nvp<T>("x1", bounds.x1),   // param x1
            cereal::make_nvp<T>("y1", bounds.y1),   // param y1
            cereal::make_nvp<T>("x2", bounds.x2),   // param x2
            cereal::make_nvp<T>("y2", bounds.y2));  // param y2
}

}  // namespace cereal
#endif