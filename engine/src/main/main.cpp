
#include <iostream>

#include "../../include/Geometry/String.hpp"
#include "../../include/Geometry/Vector.hpp"
#include "../../include/Structures/System.hpp"

int main(int argc, char* argv[]) {
    phy::System system = phy::System(false, 0.0f, 1.0f);

    for (int i = 0; i < 10; i++) { system.addCorpse(phy::Circle(10.0f * i, 0.0f, 10.0f)); }

    for (int i = 0; i < 10; i++) {
        system.Step();
        std::cout << "Step " << gmt::to_string(i) << std::endl;

        for (int j = 0; j < system.get_corpses_size(); j++) { std::cout << gmt::to_string(system.get_corpse(j)->get_pos()) << std::endl; }
    }

    return 0;
}