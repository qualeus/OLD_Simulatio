#ifndef Structures_HPP
#define Structures_HPP

struct Spawner {
    float corpse_color[4] = {};
    int spawn_type;               // 0 => default, 1 => explosion 2 => conglomerate
    int corpse_type;              // 0 = circle, 1 = circle poly, 2 = ovale poly, 3 = ...
    int duration;                 // in number of iteration (-1 = infinity)
    int interval;                 // Spawner interval
    int interval_counter;         // number of frames between each spawn
    float positionX;              // Spawner position X
    float positionY;              // Spawner position Y
    int corpse_number;            // number of corpse spawned each time
    float corpse_mass;            // Corpse mass
    float corpse_mass_random;     // Corpse mass random
    float corpse_damping;         // Corpse damping
    float corpse_damping_random;  // Corpse damping random
    float corpse_radiusX;         // radius for circles
    float corpse_radiusY;         // horizontal radius fors
    float corpse_rotation;
    float corpse_rotation_random;
    bool corpse_fixed;
    bool corpse_etherial;
    bool corpse_tied;
    int corpse_points_number;       // number of points for each shape
    float corpse_radius_randomX;    // radius interval of randomisation
    float corpse_radius_randomY;    // radius interval of randomisation
    float corpse_position_randomX;  // position interval of randomisation
    float corpse_position_randomY;  // position interval of randomisation
    float launch_power;             // launch power
    float launch_random;            // laund power random interval
    float launch_direction;         // corpse direction in degree
    float launch_direction_random;  // direction random
    float launch_rotation_power;    // rotation power
    float launch_rotation_random;   // rotation random

    Spawner() {
        corpse_color[0] = 39.0f;
        corpse_color[1] = 174.0f;
        corpse_color[2] = 96.0f;
        corpse_color[3] = 255.0f;
        spawn_type = 1;
        corpse_type = 1;
        positionX = 0.0f;
        positionY = 0.0f;
        corpse_number = 20;
        corpse_mass = 50.0f;
        corpse_mass_random = 0.0f;
        corpse_damping = 1.0f;
        corpse_damping_random = 0.0f;
        duration = 1;
        interval = 1;
        corpse_fixed = false;
        corpse_etherial = true;
        corpse_tied = false;
        corpse_radiusX = 100.0f;
        corpse_radiusY = 100.0f;
        corpse_rotation = 0.0f;
        corpse_rotation_random = 360.0f;
        corpse_points_number = 10;
        corpse_radius_randomX = 10.0f;
        corpse_radius_randomY = 10.0f;
        corpse_position_randomX = 200.0f;
        corpse_position_randomY = 200.0f;
        launch_power = 30.0f;
        launch_random = 60.0f;
        launch_direction = 0.0f;
        launch_direction_random = 360.0f;
        launch_rotation_power = 0.0f;
        launch_rotation_random = 20.0f;
        interval_counter = interval;
    }
};

#endif