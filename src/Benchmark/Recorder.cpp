#include "../../include/Benchmark/Recorder.hpp"

namespace bmk {

Recorder *Recorder::last = &root;
Recorder Recorder::root = Recorder(&perf);

Recorder::Recorder(BlockData *bloc) : Performance(bloc), parent(last) { last = this; }
Recorder ::~Recorder() {
    if (this == parent) { return; }
    last = parent;
    Performance *self = this;

    End();

    parent->childs.emplace_back(std::move(*self));
}

}  // namespace bmk