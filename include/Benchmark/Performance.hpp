#ifndef Performance_HPP
#define Performance_HPP

#include "BlockData.hpp"

namespace bmk {

class Performance {

   public:
    std::chrono::time_point<std::chrono::high_resolution_clock> begining;
    std::chrono::time_point<std::chrono::high_resolution_clock> ending;
    std::shared_ptr<BlockData> data;
    std::vector<Performance> childs;
    Performance(std::shared_ptr<BlockData> data);
    Performance(Performance &&other);
    ~Performance();
    void End();
    void ResetChilds();
    double Time();
};

}  // namespace bmk

#endif