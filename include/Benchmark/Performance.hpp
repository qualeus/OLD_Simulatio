#ifndef Performance_HPP
#define Performance_HPP

#include "BlockData.hpp"

namespace bmk {

class Performance {
   protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> begining;
    std::chrono::time_point<std::chrono::high_resolution_clock> ending;

   public:
    BlockData *data;
    std::vector<Performance> childs;
    Performance(BlockData *data);
    Performance(Performance &&other);
    ~Performance();
    void End();
    double Time();
};

}  // namespace bmk

#endif