#ifndef Geometry_HPP
#define Geometry_HPP

#include "../Benchmark/Recorder.hpp"
#include "Bounds.hpp"
#include "Config.hpp"
#include "Maths.hpp"
#include "Vector.hpp"
#include "Vertices.hpp"

namespace gmt {

#ifndef PRECISION_SET
#define PRECISION_SET
typedef float SIMULATIO_PRECISION;
#endif

#define MIN_DAMPING 0.001
#define MAX_DAMPING 100
#define MAX_GRAVITY_DISTANCE 1000000000 /* To avoid strange behaviours when comparing big numbers */

using UnitI = SIMULATIO_PRECISION;
using VectorI = Vector<SIMULATIO_PRECISION>;
using BoundsI = Bounds<SIMULATIO_PRECISION>;
using VerticesI = Vertices<SIMULATIO_PRECISION>;
using CollisionI = Collision<SIMULATIO_PRECISION>;

}  // namespace gmt
#endif