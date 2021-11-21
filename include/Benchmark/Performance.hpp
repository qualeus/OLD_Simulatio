#ifndef Performance_HPP
#define Performance_HPP

#include "BlockData.hpp"

namespace bmk {

class Performance {
    std::chrono::time_point<std::chrono::high_resolution_clock> beginning;
    std::chrono::time_point<std::chrono::high_resolution_clock> ending;

   public:
    std::shared_ptr<BlockData> data;
    std::vector<Performance> childs;
    Performance(std::shared_ptr<BlockData> data);
    Performance(Performance &&other);
    ~Performance();
    void End();
    void ResetChilds();
    double Time();
    double Beginning();
    double Ending();
};

static std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

}  // namespace bmk

#endif