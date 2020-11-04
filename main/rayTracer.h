#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"

class RayTracer {
public:
  RayTracer(objects::Model *model);

  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  TraceResult rayTrace(const core::Ray &ray, core::RayHitData *hitData);
};