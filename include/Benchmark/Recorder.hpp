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
    Recorder(BlockData *bloc);
    ~Recorder();
    void Reset();
};

static BlockData perf{"Global Root", __FILE__, __LINE__};

}  // namespace bmk

#define YTRACE(name, x, y)                                          \
    /*static*/ BlockData __block__##x##y{name, __FILE__, __LINE__}; \
    bmk::Recorder __perf__##x##y{&__block__##x##y};
#define XTRACE(name, x, y) YTRACE(name, x, y)
#define TRACE(name, x) XTRACE(name, x, __COUNTER__)
#define Record(name) TRACE(name, __RECORDER__)

#endif
