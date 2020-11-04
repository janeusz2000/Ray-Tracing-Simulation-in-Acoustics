#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"
#include "obj/objects.h"

class RayTracer {
public:
  RayTracer(Model *model);

  // TODO: write comment about hit data, that its required
  // return
  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  TraceResult rayTrace(const core::Ray &ray, core::RayHitData *hitData);
};