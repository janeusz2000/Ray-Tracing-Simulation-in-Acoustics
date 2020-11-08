#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"
#include "obj/objects.h"

class RayTracer {
public:
  RayTracer(ModelInterface *model);
  enum class TraceResult { HIT_TRIANGLE, WENT_OUTSIDE_OF_SIMULATION_SPACE };
  // |hitData| is modified to hold information where ray hit the triangle,
  // or where it went outside the simulation
  [[nodiscard]] TraceResult rayTrace(const core::Ray &ray,
                                     core::RayHitData *hitData);
};