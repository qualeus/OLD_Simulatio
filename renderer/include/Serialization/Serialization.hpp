#ifndef Serialization_HPP
#define Serialization_HPP

#include <Structures/System.hpp>
#include <cereal/access.hpp>
#include <cereal/details/traits.hpp>

namespace cereal {

template <class Archive>
void save(Archive& archive, const phy::System& system) {
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

    archive(cereal::make_nvp<bool>("gravity", gravity),                          // param gravity
            cereal::make_nvp<bool>("enable_limits", enable_limits),              // param enable_limits
            cereal::make_nvp<gmt::UnitI>("LS", LS),                              // param LS
            cereal::make_nvp<gmt::UnitI>("G", G),                                // param G
            cereal::make_nvp<gmt::UnitI>("force_x", force_x),                    // param force_x
            cereal::make_nvp<gmt::UnitI>("force_y", force_y),                    // param force_y
            cereal::make_nvp<gmt::UnitI>("dt", dt),                              // param dt
            cereal::make_nvp<double>("t", t),                                    // param t
            cereal::make_nvp<int>("collision_accuracy", collision_accuracy),     // param collision_accuracy
            cereal::make_nvp<int>("constraint_accuracy", constraint_accuracy));  // param constraint_accuracy
}

template <class Archive>
void load(Archive& archive, phy::System& system) {
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

    archive(cereal::make_nvp<bool>("gravity", gravity),                          // param gravity
            cereal::make_nvp<bool>("enable_limits", enable_limits),              // param enable_limits
            cereal::make_nvp<gmt::UnitI>("LS", LS),                              // param LS
            cereal::make_nvp<gmt::UnitI>("G", G),                                // param G
            cereal::make_nvp<gmt::UnitI>("force_x", force_x),                    // param force_x
            cereal::make_nvp<gmt::UnitI>("force_y", force_y),                    // param force_y
            cereal::make_nvp<gmt::UnitI>("dt", dt),                              // param dt
            cereal::make_nvp<double>("t", t),                                    // param t
            cereal::make_nvp<int>("collision_accuracy", collision_accuracy),     // param collision_accuracy
            cereal::make_nvp<int>("constraint_accuracy", constraint_accuracy));  // param constraint_accuracy

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

}  // namespace cereal
#endif