#include "../../include/Benchmark/Performance.hpp"

namespace bmk {

Performance::Performance(BlockData *data) : data(data) { begining = std::chrono::high_resolution_clock::now(); }
Performance::Performance(Performance &&other) : data(other.data) {
    begining = other.begining;
    ending = other.ending;
    childs = std::move(other.childs);
}
Performance ::~Performance() { End(); }

void Performance::End() { ending = std::chrono::high_resolution_clock::now(); }

double Performance::Time() {
    std::chrono::duration<double, std::milli> ms = ending - begining;
    return ms.count();
}

}  // namespace bmk