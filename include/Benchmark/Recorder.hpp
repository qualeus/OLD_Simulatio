#ifndef Recorder_HPP
#define Recorder_HPP

#include "Performance.hpp"

namespace bmk {

class Recorder : public Performance {
   private:
    Recorder *parent;

   public:
    static Recorder root;
    static Recorder *last;
    Recorder(std::shared_ptr<BlockData> bloc);
    ~Recorder();
    void Reset();
};

static BlockData perf = BlockData("Global Root", __FILE__, __LINE__);

}  // namespace 

#define Record(name) Recorder __perf__{std::make_shared<bmk::BlockData>(bmk::BlockData(name, __FILE__, __LINE__))};

#endif
